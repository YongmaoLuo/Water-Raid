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

    if (ioctl(videoFd, WATER_VIDEO_WRITE_BOUNDARY, &arg)) {
        perror("ioctl(WATER_VIDEO_WRITE_BOUNDARY) failed");
        return;
    }
}

void WaterDriver::writePosition(int videoFd,Position position,int type, int index) {
    water_video_arg_position arg;
    arg.pos=position;
    arg.type=type;
    arg.index=index;

    if (ioctl(videoFd, WATER_VIDEO_WRITE_POSITION, &arg)) {
        perror("ioctl(WATER_VIDEO_WRITE_POSITION) failed");
        return;
    }

}

void WaterDriver::writeFuel(int videoFd, int fuel) {
    water_video_arg_fuel arg;
    arg.fuel=fuel;

    if (ioctl(videoFd, WATER_VIDEO_WRITE_FUEL, &arg)) {
        perror("ioctl(WATER_VIDEO_WRITE_FUEL) failed");
        return;
    }
}

void WaterDriver::writeScore(int videoFd, int score) {
    water_video_arg_score arg;
    arg.score=score;

    if (ioctl(videoFd, WATER_VIDEO_WRITE_SCORE, &arg)) {
        perror("ioctl(WATER_VIDEO_SET_SCORE) failed");
        return;
    }
}

void WaterDriver::initBackground(int videoFd) {
    water_video_arg_init arg;
    arg.scorePos.x=480;
    arg.scorePos.y=(440 << 1) + 1;
    arg.digit1Pos.x=525;
    arg.digit1Pos.y=(440 << 1) + 1;
    arg.digit2Pos.x=550;
    arg.digit2Pos.y=(440 << 1) + 1;
    arg.digit3Pos.x=575;
    arg.digit3Pos.y=(440 << 1) + 1;
    arg.fuelPos.x=320;
    arg.fuelPos.y=(440 << 1) + 1;
    arg.indicatorPos.x=320;
    arg.indicatorPos.y=(438 << 1) + 1;
    if (ioctl(videoFd, WATER_VIDEO_INIT,&arg)) {
        perror("ioctl(WATER_VIDEO_INIT) failed");
        return;
    }
}