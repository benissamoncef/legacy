/*E*/
/*  Fichier : $Id: xzae197sp.prc,v 1.6 2020/07/20 16:02:32 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/07/20 16:02:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae197sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lit les coordonnées relative à une autoroute et un PR
*
---------------------------------------------------------------
* HISTORIQUE :
* ABE 01/07/2020	: Creation DEM 149 V1.1
* ABE 16/07/2020	: Modification suite recette DEM 149 V1.2
* PNI 17/07/2020	: projection des sens en bretelle sur section courante + autoroute inversée DEM 149 V1.3
* ABE 20/07/2020	: Ajout test sur nom_patrouille !="" DEM 149 V1.5
--------------------------------------------------------------*/

/*X*/
/*-------------------------------------------------------
* Sequence d'appel
* SP	xzae197
* 
* Arguments en entree
*
* Arguments en sortie
*
* char *	 	va_radio_out
* CS_DATETIME * 	va_heure_succes_out
*
* Code retour
* XDC_OK
* XDC_NOK		: fiche non trouvee
* XDC_ARG_INV		: argument d'entree  null
* XZAEC_FMC		_PAS_RETYPAGE : retypage non autorise (16)
* < 0			: erreur sybase 
* 
* Conditions d'utilisation
* ne fonctionne pas sur pc simplifie 
* 
* Fonction
*
----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE197' and type = 'P')
	drop procedure XZAE197
go

create procedure XZAE197		

as
	select CFG..ANN_FIC.radio,
			EXP..ACT_GEN.heure_succes,
			EXP..FMC_HIS_DER.autoroute,
			EXP..FMC_HIS_DER.PR,
			EXP..FMC_HIS_DER.sens
	FROM CFG..ANN_FIC,EXP..ACT_GEN,EXP..ACT_SIS,EXP..FMC_HIS_DER,CFG..RES_AUT_INV
	where CFG..ANN_FIC.nom like "%"+EXP..ACT_SIS.nom_patrouille+"%"
		and EXP..ACT_GEN.type=XDC_ACT_PAT_SIS
		and EXP..ACT_GEN.heure_echec=NULL
		and EXP..ACT_GEN.heure_fin=NULL
		and EXP..ACT_GEN.heure_succes is not NULL
		and CFG..ANN_FIC.nom like "FOURGON%"		
		and EXP..ACT_GEN.evenement=EXP..FMC_HIS_DER.numero
		and EXP..ACT_GEN.cle=EXP..FMC_HIS_DER.cle
		and EXP..ACT_SIS.actio=EXP..ACT_GEN.numero
		and CFG..ANN_FIC.inhibition=XDC_FAUX
		and EXP..FMC_HIS_DER.sens in (1,2)
		and EXP..FMC_HIS_DER.autoroute=CFG..RES_AUT_INV.numero
		and rtrim(EXP..ACT_SIS.nom_patrouille)!=""
		and inversee=1
union
	select CFG..ANN_FIC.radio,
			EXP..ACT_GEN.heure_succes,
			EXP..FMC_HIS_DER.autoroute,
			EXP..FMC_HIS_DER.PR,
			1
	FROM CFG..ANN_FIC,EXP..ACT_GEN,EXP..ACT_SIS,EXP..FMC_HIS_DER,CFG..RES_AUT_INV
	where CFG..ANN_FIC.nom like "%"+EXP..ACT_SIS.nom_patrouille+"%"
		and EXP..ACT_GEN.type=XDC_ACT_PAT_SIS
		and EXP..ACT_GEN.heure_echec=NULL
		and EXP..ACT_GEN.heure_fin=NULL
		and EXP..ACT_GEN.heure_succes is not NULL
		and CFG..ANN_FIC.nom like "FOURGON%"		
		and EXP..ACT_GEN.evenement=EXP..FMC_HIS_DER.numero
		and EXP..ACT_GEN.cle=EXP..FMC_HIS_DER.cle
		and EXP..ACT_SIS.actio=EXP..ACT_GEN.numero
		and CFG..ANN_FIC.inhibition=XDC_FAUX
		and EXP..FMC_HIS_DER.sens in (3,7)
		and EXP..FMC_HIS_DER.autoroute=CFG..RES_AUT_INV.numero
		and rtrim(EXP..ACT_SIS.nom_patrouille)!=""
		and inversee=1

union
	select CFG..ANN_FIC.radio,
			EXP..ACT_GEN.heure_succes,
			EXP..FMC_HIS_DER.autoroute,
			EXP..FMC_HIS_DER.PR,
			2
	FROM CFG..ANN_FIC,EXP..ACT_GEN,EXP..ACT_SIS,EXP..FMC_HIS_DER,CFG..RES_AUT_INV
	where CFG..ANN_FIC.nom like "%"+EXP..ACT_SIS.nom_patrouille+"%"
		and EXP..ACT_GEN.type=XDC_ACT_PAT_SIS
		and EXP..ACT_GEN.heure_echec=NULL
		and EXP..ACT_GEN.heure_fin=NULL
		and EXP..ACT_GEN.heure_succes is not NULL
		and CFG..ANN_FIC.nom like "FOURGON%"		
		and EXP..ACT_GEN.evenement=EXP..FMC_HIS_DER.numero
		and EXP..ACT_GEN.cle=EXP..FMC_HIS_DER.cle
		and EXP..ACT_SIS.actio=EXP..ACT_GEN.numero
		and CFG..ANN_FIC.inhibition=XDC_FAUX
		and EXP..FMC_HIS_DER.sens in (4,8)
		and EXP..FMC_HIS_DER.autoroute=CFG..RES_AUT_INV.numero
		and rtrim(EXP..ACT_SIS.nom_patrouille)!=""
		and inversee=1

union
	select CFG..ANN_FIC.radio,
			EXP..ACT_GEN.heure_succes,
			EXP..FMC_HIS_DER.autoroute,
			EXP..FMC_HIS_DER.PR,
			EXP..FMC_HIS_DER.sens%2+1
	FROM CFG..ANN_FIC,EXP..ACT_GEN,EXP..ACT_SIS,EXP..FMC_HIS_DER,CFG..RES_AUT_INV
	where CFG..ANN_FIC.nom like "%"+EXP..ACT_SIS.nom_patrouille+"%"
		and EXP..ACT_GEN.type=XDC_ACT_PAT_SIS
		and EXP..ACT_GEN.heure_echec=NULL
		and EXP..ACT_GEN.heure_fin=NULL
		and EXP..ACT_GEN.heure_succes is not NULL
		and CFG..ANN_FIC.nom like "FOURGON%"		
		and EXP..ACT_GEN.evenement=EXP..FMC_HIS_DER.numero
		and EXP..ACT_GEN.cle=EXP..FMC_HIS_DER.cle
		and EXP..ACT_SIS.actio=EXP..ACT_GEN.numero
		and CFG..ANN_FIC.inhibition=XDC_FAUX
		and EXP..FMC_HIS_DER.sens in (1,2)
		and EXP..FMC_HIS_DER.autoroute=CFG..RES_AUT_INV.numero
		and rtrim(EXP..ACT_SIS.nom_patrouille)!=""
		and inversee=-1
union
	select CFG..ANN_FIC.radio,
			EXP..ACT_GEN.heure_succes,
			EXP..FMC_HIS_DER.autoroute,
			EXP..FMC_HIS_DER.PR,
			2
	FROM CFG..ANN_FIC,EXP..ACT_GEN,EXP..ACT_SIS,EXP..FMC_HIS_DER,CFG..RES_AUT_INV
	where CFG..ANN_FIC.nom like "%"+EXP..ACT_SIS.nom_patrouille+"%"
		and EXP..ACT_GEN.type=XDC_ACT_PAT_SIS
		and EXP..ACT_GEN.heure_echec=NULL
		and EXP..ACT_GEN.heure_fin=NULL
		and EXP..ACT_GEN.heure_succes is not NULL
		and CFG..ANN_FIC.nom like "FOURGON%"		
		and EXP..ACT_GEN.evenement=EXP..FMC_HIS_DER.numero
		and EXP..ACT_GEN.cle=EXP..FMC_HIS_DER.cle
		and EXP..ACT_SIS.actio=EXP..ACT_GEN.numero
		and CFG..ANN_FIC.inhibition=XDC_FAUX
		and EXP..FMC_HIS_DER.sens in (3,7)
		and EXP..FMC_HIS_DER.autoroute=CFG..RES_AUT_INV.numero
		and rtrim(EXP..ACT_SIS.nom_patrouille)!=""
		and inversee=-1

union
	select CFG..ANN_FIC.radio,
			EXP..ACT_GEN.heure_succes,
			EXP..FMC_HIS_DER.autoroute,
			EXP..FMC_HIS_DER.PR,
			1
	FROM CFG..ANN_FIC,EXP..ACT_GEN,EXP..ACT_SIS,EXP..FMC_HIS_DER,CFG..RES_AUT_INV
	where CFG..ANN_FIC.nom like "%"+EXP..ACT_SIS.nom_patrouille+"%"
		and EXP..ACT_GEN.type=XDC_ACT_PAT_SIS
		and EXP..ACT_GEN.heure_echec=NULL
		and EXP..ACT_GEN.heure_fin=NULL
		and EXP..ACT_GEN.heure_succes is not NULL
		and CFG..ANN_FIC.nom like "FOURGON%"		
		and EXP..ACT_GEN.evenement=EXP..FMC_HIS_DER.numero
		and EXP..ACT_GEN.cle=EXP..FMC_HIS_DER.cle
		and EXP..ACT_SIS.actio=EXP..ACT_GEN.numero
		and CFG..ANN_FIC.inhibition=XDC_FAUX
		and EXP..FMC_HIS_DER.sens in (4,8)
		and EXP..FMC_HIS_DER.autoroute=CFG..RES_AUT_INV.numero
		and rtrim(EXP..ACT_SIS.nom_patrouille)!=""		
		and inversee=-1



return XDC_OK
		
go

		
