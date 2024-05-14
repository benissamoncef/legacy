cd ~/exec
cp -p tasrv.x tasrv_DP.x
cp -p tacli.x tacli_DP.x
cp -p teini.x teini_DP.x
cp -p tapur.x tapur_DP.x
nohup tasrv_DP.x migrazur &
nohup tacli_DP.x migrazur &
nohup teini_DP.x migrazur &
nohup tapur_DP.x migrazur &
