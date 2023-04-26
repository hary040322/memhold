#!/bin/bash
set -x

INST_DIR="/usr/local/memhold"
CONF_JSON="/root/config/memhold/config.json"

touch /var/log/memhold.log
mkdir -p /root/config/memhold/
cp -f $INST_DIR/meta.json /root/config/memhold/
chmod 777 -R /root/config/
dpkg -i $INST_DIR/*.deb >> /var/log/memhold.log 2>&1
# gcc clog.c alloc_mem.c -o blloc
#export MEM_HOLD_MB=112 && /usr/local/memhold/blloc

if [ -f $CONF_JSON ]; then
  mem=`cat $CONF_JSON | jq '.MemMax' | sed 's/\"//g'`
else
  mem=100
fi

step=`cat $CONF_JSON | jq '.SecStep' | sed 's/\"//g'`
if [ xnull == x$step ]; then
  step=50
fi

key=`grep Environment=MEM_HOLD_MB= $INST_DIR/memhold.service`
sed -i "s/${key}/Environment=MEM_HOLD_MB=${mem} SEC_STEP_MB=${step}/g" $INST_DIR/memhold.service
export MEM_HOLD_MB=$mem
#/usr/local/memhold/blloc
cp -f $INST_DIR/memhold.service /etc/systemd/system/
systemctl daemon-reload
systemctl restart memhold.service
#tar -zcvf memhold.0.4.tar.gz memhold/
