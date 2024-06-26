FROM ubuntu:22.04

# install required tools
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get install -y -q \
  tzdata \
  cmake \
  git \
  hwdata \
  libglib2.0-0 \
  libnuma1 \
  libpixman-1-0 \
  linux-tools-virtual \
  gcc \
  g++ \
  pkg-config \
  libssl-dev \
  libdbus-1-dev \
  libglib2.0-dev \
  libavahi-client-dev \
  ninja-build \
  python3 \
  python3-venv \
  python3-dev \
  python3-pip \
  unzip \
  libgirepository1.0-dev \
  libcairo2-dev \
  libreadline-dev \
  libusb-1.0-0

RUN update-alternatives --install /usr/local/bin/usbip usbip `ls /usr/lib/linux-tools/*/usbip | tail -n1` 20

RUN : \
  && apt-get install -y \
  wget flex bison gperf ccache libffi-dev dfu-util \
  && apt-get autoremove -y \
  && rm -rf /var/lib/apt/lists/* \
  && update-alternatives --install /usr/bin/python python /usr/bin/python3 10 \
  && :

# ESP-IDF
ENV ISP_VERSION=v5.1.2
ENV IDF_PATH=/opt/esp/idf
ENV IDF_TOOLS_PATH=/opt/esp
ENV ISP_TARGETS=esp32

# Clone esp_idf
RUN : \
  && git clone --recursive --depth=1 --shallow-submodules --jobs 8 -b $ISP_VERSION https://github.com/espressif/esp-idf.git $IDF_PATH \
  && git config --system --add safe.directory $IDF_PATH \
  && :

RUN : \
  && $IDF_PATH/tools/idf_tools.py --non-interactive install required --targets=${ISP_TARGETS} \
  && $IDF_PATH/tools/idf_tools.py --non-interactive install cmake \
  && $IDF_PATH/tools/idf_tools.py --non-interactive install-python-env \
  && rm -rf $IDF_TOOLS_PATH/dist \
  && :

# Ccache is installed, enable it by default
ENV IDF_CCACHE_ENABLE=1

# The constraint file has been downloaded and the right Python package versions installed. No need to check and
# download this at every invocation of the container.
ENV IDF_PYTHON_CHECK_CONSTRAINTS=no

# ESP MATTER
ENV ESP_MATTER_VERSION=release/v1.2
ENV ESP_MATTER_PATH=/opt/esp/matter

WORKDIR $ESP_MATTER_PATH
RUN : \
  && git clone --depth 1 --jobs 8 https://github.com/espressif/esp-matter.git -b $ESP_MATTER_VERSION . \
  && git submodule update --init --depth 1 \
  && cd ./connectedhomeip/connectedhomeip \
  && ./scripts/checkout_submodules.py --platform esp32 linux --shallow \
  && :

WORKDIR $ESP_MATTER_PATH
RUN : \  
  && . $IDF_PATH/export.sh \
  && ./install.sh \
  && :

# Add main user
ENV CONTAINER_USER=root
ARG CONTAINER_USER=$CONTAINER_USER
# ENV USER_UID=1000
# ENV USER_GID=$USER_UID
# RUN groupadd --gid $USER_GID $CONTAINER_USER \
#     && adduser --uid $USER_UID --gid $USER_GID --disabled-password --gecos "" ${CONTAINER_USER} \
#     && usermod -a -G dialout $CONTAINER_USER \
#     && apt-get update \
#     && apt-get install -y sudo \
#     && echo $CONTAINER_USER ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$CONTAINER_USER \
#     && chmod 0440 /etc/sudoers.d/$CONTAINER_USER

# RUN adduser ${CONTAINER_USER} sudo

USER ${CONTAINER_USER}
ENV USER=${CONTAINER_USER}
WORKDIR /home/${CONTAINER_USER}

# # don't care about safe directory in a encapsulateds docker env
# RUN git config --global --add safe.directory *

RUN echo "source $IDF_PATH/export.sh > /dev/null 2>&1" >> ~/.bashrc
RUN echo "source $ESP_MATTER_PATH/export.sh > /dev/null 2>&1" >> ~/.bashrc
RUN echo "export LC_ALL=C" >> ~/.bashrc

CMD ["/bin/bash", "-c"]