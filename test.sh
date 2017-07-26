#!/bin/bash

#dd if=./rollback_blob bs=9000 > /dev/tcp/10.0.0.42/665
dd if=./build/Serverless bs=9000 > /dev/tcp/10.0.0.42/666
