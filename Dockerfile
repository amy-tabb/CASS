FROM amytabb/ubuntu18-ceres-opencv4-contrib:latest

MAINTAINER Amy Tabb

#cass
WORKDIR /installed_libs/

RUN git clone https://github.com/amy-tabb/CASS.git

WORKDIR /installed_libs/CASS/build/

RUN cmake ../src

RUN make

RUN make install

## exiftools

RUN apt-get -y install exiftool

WORKDIR /host_dir/





