//
// Created by Yongmao Luo on 4/29/22.
//
#include "driver.h"
#include "../VideoDriver/water_video.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>

int shift=0;

void WaterDriver::writeBoundary(int videoFd, BoundaryInRow boundary) {
    water_video_arg_boundary arg;
    arg.boundary=boundary;
    shift=1-shift;
    arg.shift=shift;

    //water_video_arg_boundary writeTemp=boundary;
    if (ioctl(videoFd, WATER_VIDEO_WRITE_BOUNDARY, &arg)) {
        perror("ioctl(WATER_VIDEO_SET_COORDINATE) failed");
        return;
    }
}

void WaterDriver::writePlanePosition(Position position) {}