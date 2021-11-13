#FROM python:3.9-buster
#FROM debian:stable
FROM debian:stable-slim

RUN apt-get update && apt-get install -yq \
    xsel=1.2.0+git9bfc13d.20180109-1 \
    unzip=6.0-23+deb10u2 \
    python-pytest=3.10.1-2 \
    libgconf-2-4=3.2.6-5 \
    libncurses5=6.1+20181013-2+deb10u2 \
    libxml2-dev=2.9.4+dfsg1-7+deb10u1 \
    libxslt-dev \
    libz-dev \
    xclip=0.13-1
RUN apt-get install -y curl xvfb wget python3 python3-pip
RUN apt install -y libcairo2-dev pkg-config python3-dev
#RUN locate pip3
#RUN dpkg -L python3-pip


RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
COPY ./requirements.txt /usr/src/app/
RUN /usr/bin/pip3 install --no-cache-dir -r requirements.txt
COPY ./ /usr/src/app
CMD ["python3", "sheetsHandler.py"]
