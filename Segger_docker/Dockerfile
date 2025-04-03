FROM ubuntu:22.04

# Set non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install required dependencies
RUN apt-get update && apt-get install -y \
    curl \
    file \
    wget \
    libxrender1 \
    libxcb-render0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-randr0 \
    libxcb-xfixes0 \
    libxcb-sync1 \
    libxcb-shm0 \
    libxcb-icccm4 \
    libxcb-keysyms1 \
    libxcb-image0 \
    libxkbcommon0 \
    libxkbcommon-x11-0 \
    libfontconfig1 \
    libfreetype6 \
    libxext6 \
    libx11-6 \
    libxcb1 \
    libx11-xcb1 \
    libsm6 \
    libice6 \
    libglib2.0-0 \
    tar \
    && rm -rf /var/lib/apt/lists/*


# # Download, extract, and install SEGGER Embedded Studio locally and then copy - https://www.segger.com/downloads/embedded-studio/Setup_EmbeddedStudio_ARM_v562_linux_x64.tar.gz
# Locally it's in /usr/share/segger_embedded_studio_for_arm_5.62/ 
# Copy this folder from host to same dir as your docker file
COPY segger_embedded_studio_for_arm_5.62/ /home/segger/

# ENV PATH="/home/segger/bin/emBuild:${PATH}"
RUN echo "export PATH=$PATH:/home/segger/bin/" >> ~/.bashrc

#now you can build seggar project using emBuild .project -config Debug/Release


