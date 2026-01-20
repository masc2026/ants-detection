#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "DatasetGenerator.hpp"
#include <filesystem>
#include <iostream>

using namespace cv;
using namespace std;
using namespace masc::annotations;
namespace fs = std::filesystem;

class DatasetGeneratorTest : public ::testing::Test {
protected:
    DatasetGenerator gen;
    cv::Mat maske_bw;
    cv::VideoCapture cap;

    std::string set_path;
    std::string clip_path;
    std::string json_path;
    std::string maske_bw_path;
    std::string debug_path;
    std::string yolo_path;
    ErkennungKonfig config;

    void SetUp() override {
        config.debug = 0;
        config.gauss_deviation = 17;
        config.thresh_thresh = 16.0;
        config.boundingbox_min_area = 700;
        config.kontrast_alpha = 0.9;
        config.kontrast_beta = 0.0;
        
        std::cout << "🏁 Starte Testcase..." << std::endl;
        
        // PROJECT_SOURCE_DIR wird über CMake gesetzt
        set_path = std::string(PROJECT_SOURCE_DIR) + "/Test/data/";
        clip_path = set_path + "clip640x424.mp4";
        maske_bw_path = set_path + "maske640x424.png";
        json_path = set_path + "clip.json";
        debug_path = set_path + "debug/";
        yolo_path = set_path + "yolo/";
        
        cap.open(clip_path);
        ASSERT_TRUE(cap.isOpened()) << "Video konnte nicht geöffnet werden: " << clip_path;
        
        maske_bw = cv::imread(maske_bw_path, cv::IMREAD_GRAYSCALE);
        ASSERT_FALSE(maske_bw.empty()) << "Maske konnte nicht geladen werden: " << maske_bw_path;
        
        gen.initReferenzFrames(json_path, clip_path, maske_bw, config);
        fs::create_directories(debug_path);
    }

    void TearDown() override {
        cap.release();
    }
};

TEST_F(DatasetGeneratorTest, ReferenzFrameLaden) {
    cv::Mat ref_frame = gen.ladeReferenzFrame(clip_path, 0, maske_bw, config);
    EXPECT_FALSE(ref_frame.empty()) << "Referenz Frame 0 wurde nicht geöffnet";
}

TEST_F(DatasetGeneratorTest, BoundingBoxFrame) {
    std::string test_debug_path = debug_path + "testBoundingBoxFrame/";
    fs::create_directories(test_debug_path);
    std::vector<std::tuple<int, int, int>> frameDaten = {
        {26059, 2, 3}
    };
   
    for (const auto& [frame_index, erwarteteAnzahlMin, erwarteteAnzahlMax] : frameDaten) {
        int refIdx = gen.referenzFrameFuerIndex(json_path, frame_index);
        cv::Mat ref_frame = DatasetGenerator::getReferenzFrame(refIdx); // Ggf. Objektinstanz nutzen, falls nicht statisch
        EXPECT_FALSE(ref_frame.empty()) << "Referenz Frame " << refIdx << " für Frame " << frame_index << " wurde nicht geöffnet";
        
        auto [konturen, boxen, gefilterte_boxen, frame] = gen.boundingBoxes(cap, frame_index, maske_bw, ref_frame, config);
        int n = (int)gefilterte_boxen.size();
        
        EXPECT_TRUE(n >= erwarteteAnzahlMin && n <= erwarteteAnzahlMax) 
            << "Frame: " << frame_index << " - Anzahl der Boxen ist " << n 
            << " und liegt nicht im erwarteten Bereich (" << erwarteteAnzahlMin << " – " << erwarteteAnzahlMax << ")";
            
        gen.debugDataset(konturen, boxen, gefilterte_boxen, ref_frame, frame, frame_index, refIdx, test_debug_path, config);
    }
}

TEST_F(DatasetGeneratorTest, BoundingBoxFrameSerie) {
    std::string test_debug_path = debug_path + "testBoundingBoxFrameSerie/";
    fs::create_directories(test_debug_path);
    std::vector<std::tuple<int, int, int>> frameDaten = {
        {914, 0, 2}, {10423, 2, 2}, {10447, 2, 3}, {26059, 2, 3}, {35456, 1, 3}, {35683, 2, 3}, {35780, 2, 2}
    };
   
    for (const auto& [frame_index, erwarteteAnzahlMin, erwarteteAnzahlMax] : frameDaten) {
        int refIdx = gen.referenzFrameFuerIndex(json_path, frame_index);
        cv::Mat ref_frame = gen.getReferenzFrame(refIdx);
        EXPECT_FALSE(ref_frame.empty()) << "Referenz Frame " << refIdx << " für Frame " << frame_index << " wurde nicht geöffnet";
        
        auto [konturen, boxen, gefilterte_boxen, frame] = gen.boundingBoxes(cap, frame_index, maske_bw, ref_frame, config);
        int n = (int)gefilterte_boxen.size();
        
        EXPECT_TRUE(n >= erwarteteAnzahlMin && n <= erwarteteAnzahlMax) 
            << "Frame: " << frame_index << " - Anzahl der Boxen ist " << n 
            << " und liegt nicht im erwarteten Bereich (" << erwarteteAnzahlMin << " – " << erwarteteAnzahlMax << ")";
            
        gen.debugDataset(konturen, boxen, gefilterte_boxen, ref_frame, frame, frame_index, refIdx, test_debug_path, config);
    }
}