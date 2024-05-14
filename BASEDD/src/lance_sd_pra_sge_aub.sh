cd ~/exec
cp -p tasrv.x tasrv_CI.x
cp -p tacli.x tacli_CI.x
cp -p teini.x teini_CI.x
cp -p tapur.x tapur_CI.x
nohup tasrv_CI.x migrazur &
nohup tacli_CI.x migrazur &
nohup teini_CI.x migrazur &
nohup tapur_CI.x migrazur &
