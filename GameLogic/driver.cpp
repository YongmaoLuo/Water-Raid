//
// Created by Yongmao Luo on 4/29/22.
//
#include "driver.h"
#include "../VideoDriver/water_video.h"
#include <unistd.h>

void WaterDriver::writeBoundary(BoundaryInRow boundary) {
    int water_video_fd;
    static const char filename[] = "/dev/vga_ball";
    if ( (water_video_fd = open(filename, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", filename);
        return -1;
    }

    if (ioctl(water_video_fd, WATER_VIDEO_WRITE_COORDINATE, &boundary)) {
        perror("ioctl(WATER_VIDEO_SET_COORDINATE) failed");
        return;
    }
}

void WaterDriver::writePlanePosition(Position position) {}