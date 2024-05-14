/*X  Fichier : $Id: confsqlpc2.bas,v 1.1 1998/01/28 16:45:15 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/01/28 16:45:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER confsqldm.bas
------------------------------------------------------
* DESCRIPTION :
* 
* cree les devices pour le district DA
* cree le user applicatif MIGRAZUR pour le district DA
* cree les bases pour le district DA
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.TORREGROSSA	21/03/95	: Creation
----------------------------------------------------*/

use master
go

-----------------------------------------------------
-- ajout du login MIGRAZUR
-----------------------------------------------------
exec sp_addlogin MIGRAZUR, MIGRAZUR, null, null, "login applicatif Migrazur"
print "ajout du login MIGRAZUR : OK"
go
 
-----------------------------------------------------
-- variables pour diskinit
-----------------------------------------------------
declare @dernier_vdevno int
declare @taille int

-----------------------------------------------------
-- configuration des devices
-----------------------------------------------------
   print "configuration des devices du DA"
   
   -- ajout du device EXP_dev
   if not exists (select name from sysdevices where name="EXP_dev")
      begin
      print "ajout du device EXP_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=500*512 --500 meg
      disk init
         name="EXP_dev",
         physname="/dev/vg00/rlvol13",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device EXP_dev : OK"

   -- ajout du device CFG_dev
   if not exists (select name from sysdevices where name="CFG_dev")
      begin
      print "ajout du device CFG_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=100*512 --100 meg
      disk init
         name="CFG_dev",
         physname="/dev/vg00/rlvol11",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device CFG_dev : OK"
   print "fin de la configuration des devices du DA"

   -- ajout du device PRC_dev
   if not exists (select name from sysdevices where name="PRC_dev")
      begin
      print "ajout du device PRC_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=100*512 --100 meg
      disk init
         name="PRC_dev",
         physname="/dev/vg00/rlvol12",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device PRC_dev : OK"
   print "fin de la configuration des devices du DA"

   -- ajout du device LOG_dev
   if not exists (select name from sysdevices where name="LOG_dev")
      begin
      print "ajout du device LOG_dev"
      select @dernier_vdevno=max(low/16777216)+1 from sysdevices
      select @taille=200*512 --100 meg
      disk init
         name="LOG_dev",
         physname="/dev/vg00/rlvol16",
         vdevno=@dernier_vdevno,
         size=@taille
      end
   print "device LOG_dev : OK"
   print "fin de la configuration des devices du DA"
   go

-----------------------------------------------------
-- configuration des bases
-----------------------------------------------------

   -- ajout de la base EXP
if exists (select name from sysdatabases where name="EXP")
drop database EXP
go
print "ajout de la base EXP"
go
create database EXP
      on EXP_dev=100 -- 100 meg
      log on LOG_dev=100 --100 meg 
go
use EXP
exec sp_changedbowner MIGRAZUR
go
use master
print "base EXP : OK"
go
   
-- ajout de la base CFG
print "ajout de la base CFG"
go
create database CFG
      on CFG_dev=50 -- 50 meg
      log on LOG_dev=50 --50 meg sur autre device
go
use CFG
exec sp_changedbowner MIGRAZUR
use master
print "base CFG : OK"
go
   
-- ajout de la base DA_RSSD
print "ajout de la base DA_RSSD"
go
create database DA_RSSD
      on PRC_dev=10 -- 10 meg
      log on LOG_dev=10 --10 meg sur autre device
go
print "base DA_RSSD : OK"
go
   
-- ajout de la base PRC
print "ajout de la base PRC"
go
create database PRC
      on PRC_dev=50 -- 50 meg
go
use PRC
exec sp_changedbowner MIGRAZUR
use master
print "base PRC : OK"
go
   
go
--------------------------------------------------------------------
-- options des bases
--------------------------------------------------------------------
use master
go
exec sp_modifylogin MIGRAZUR,defdb, PRC
go
exec sp_dboption EXP, trunc, true
go
exec sp_dboption CFG,"select into",true
go
exec sp_dboption PRC, trunc, true
go
use EXP
go
checkpoint
go
use CFG
go
checkpoint
go
use PRC
go
checkpoint
go
use master
go
if @@servername = 'SQL_CI' 
begin
	exec sp_configure "memory", 1500
	exec sp_configure "user connections", 60
	exec sp_dboption CI_RSSD,trunc,true
end
go
if @@servername = 'SQL_DA' 
begin
	exec sp_configure "total memory", 10512
	exec sp_configure "user connections", 60
	exec sp_dboption DA_RSSD,trunc,true
end
go
reconfigure
go
if @@servername = 'SQL_CI' use CI_RSSD
go
if @@servername = 'SQL_DN' use DN_RSSD
go
if @@servername = 'SQL_DA' use DA_RSSD
go
checkpoint
go
use master
go
if @@servername = 'SQL_CI' exec sp_dboption CFT,trunc,true
go
if @@servername = 'SQL_CI' exec sp_dboption SVG,trunc,true
go
if @@servername = 'SQL_CI' use CFT
go
checkpoint
go
if @@servername = 'SQL_CI' use HIS
go
checkpoint
go
if @@servername = 'SQL_CI' use SVG
go
checkpoint
go
use master
go
exec sp_dboption tempdb,trunc,true
go
use tempdb
go
checkpoint
go
sp_addserver TASRV_DA
go
