# Debian 9 image prepared for Chataigne build
FROM debian/eol:stretch

# Workaround for issue 10: https://github.com/debuerreotype/docker-debian-eol-artifacts/issues/10
RUN echo "deb http://archive.debian.org/debian/ stretch main contrib non-free\ndeb http://archive.debian.org/debian/ stretch-backports main contrib non-free\ndeb http://archive.debian.org/debian-security/ stretch/updates main contrib non-free" > /etc/apt/sources.list && apt-get update

# Install build dependencies and tools
RUN apt-get install -qyf \
        wget \
        curl \
        libcurl3-gnutls \
        libfreetype6-dev \
        libx11-dev \
        libxinerama-dev \
        libxrandr-dev \
        libxcursor-dev \
        libxcomposite-dev \
        mesa-common-dev \
        libasound2-dev \
        freeglut3-dev \
        libcurl4-gnutls-dev \
        libasound2-dev \
        libjack-dev \
        libbluetooth-dev \
        libgtk-3-dev \
        libwebkit2gtk-4.0-dev \
        libsdl2-dev \
        libfuse2 \
        libusb-1.0-0-dev \
        libhidapi-dev \
        ladspa-sdk

# Github actions/Checkout@v2 requires git >=2.18 for proper operation
RUN apt-get -t stretch-backports install git -y

# Install g++ 8 from buster (Chataigne requires C++17)
RUN echo "deb http://deb.debian.org/debian/ buster main contrib non-free\n" > /etc/apt/sources.list && apt-get update
RUN apt-get -t buster install g++ -y