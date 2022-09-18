//
// Created by Yongmao Luo on 4/29/22.
//

// The upper level driver function call
// Copyright (C) 2022  Yongmao Luo

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
    arg.scorePos.y=(30 << 1) + 1;
    arg.digit1Pos.x=525;
    arg.digit1Pos.y=(30 << 1) + 1;
    arg.digit2Pos.x=550;
    arg.digit2Pos.y=(30 << 1) + 1;
    arg.digit3Pos.x=575;
    arg.digit3Pos.y=(30 << 1) + 1;
    arg.fuelPos.x=320;
    arg.fuelPos.y=(30 << 1) + 1;
    arg.indicatorPos.x=320;
    arg.indicatorPos.y=(28 << 1) + 1;
    if (ioctl(videoFd, WATER_VIDEO_INIT,&arg)) {
        perror("ioctl(WATER_VIDEO_INIT) failed");
        return;
    }
}

void WaterDriver::playAudio(int audioFd, int index) {
    water_audio_arg arg;
    arg.index=index;

    if (ioctl(audioFd, WATER_AUDIO_PLAY, &arg)) {
        perror("ioctl(WATER_VIDEO_SET_SCORE) failed");
        return;
    }

}