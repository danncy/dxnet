PROJECT(dxnet)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)
AUX_SOURCE_DIRECTORY(./cli DIR_SRCS)
AUX_SOURCE_DIRECTORY(./restfull DIR_SRCS)

ADD_LIBRARY(interface SHARED ${DIR_SRCS})