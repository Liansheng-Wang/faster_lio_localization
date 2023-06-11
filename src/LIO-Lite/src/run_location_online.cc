
#include <unistd.h>
#include <csignal>

#include "custom_logs.hpp"
#include "laser_mapping.h"

DEFINE_bool(need_logs, true, "save logs for check");
void SigHandle(int sig) {
    lio_lite::options::FLAG_EXIT = true;
    ROS_WARN("catch sig %d", sig);
}

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    init_log();
    if(FLAGS_need_logs){
        save_log("local");
    }

    ros::init(argc, argv, "lio_lite");
    ros::NodeHandle nh;

    auto laser_mapping = std::make_shared<lio_lite::LaserMapping>();
    laser_mapping->InitROS(nh);

    signal(SIGINT, SigHandle);
    ros::Rate rate(5000);
    
    while (ros::ok()) {
        if (lio_lite::options::FLAG_EXIT) {
            break;
        }
        ros::spinOnce();
        laser_mapping->Run();
        rate.sleep();
    }

    LOG(INFO) << "finishing!";
    laser_mapping->Finish();

    lio_lite::Timer::PrintAll();

    return 0;
}
