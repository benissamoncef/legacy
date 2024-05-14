cd ~/exec
cp -p tasrv.x tasrv_VC.x
cp -p tacli.x tacli_VC.x
cp -p teini.x teini_VC.x
cp -p tapur.x tapur_VC.x
nohup tasrv_VC.x migrazur &
nohup tacli_VC.x migrazur &
nohup teini_VC.x migrazur &
nohup tapur_VC.x migrazur &
