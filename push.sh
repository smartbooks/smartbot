#!/bin/bash

export cur_home=$(cd `dirname $0`; pwd)

cd ${cur_home}

git add .

git commit -m "auto commit"

git push origin
