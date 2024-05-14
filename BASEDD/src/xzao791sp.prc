/*E*/
/*Fichier : $Id: xzao791sp.prc,v 1.1 2017/03/31 17:53:43 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:53:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao791sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao791sp.h >
*
* JMG   20/03/17        creation DEM1191
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO791SP_Rech_Echangeur
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute		Autoroute_In
* XDY_PR		PR_In
* XDY_Sens		Sens_In
* XDY_Horodate		Horodate_in
*
* PARAMETRES EN SORTIE :	
*
* XDY_Eqt		NumEch
*
* VALEUR RENDUE :
*
* Recherche le premier echangeur en amont et verifie qu'il possede une
* signalisation non inhibee
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Select dans les tables EQT_GEN (EQUIPEMENT) et EQT_ECH (DEMI-ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	02/01/17 creation
------------------------------------------------------*/


use PRC
go

if exists (select * from sysobjects where name = 'XZAO791' and type = 'P')
	drop procedure XZAO791
go

create procedure XZAO791
	@va_Eqt_in	smallint ,
   	@va_Type_out	tinyint = null output
as 
      
   /* Test des parametres d'entree :
      Si un des parametres d'entree est egal a null alors XDC_NOK   */
      
   if @va_Eqt_in = null
      return XDC_ARG_INV
   else   
      
    begin
    
	select 	@va_Type_out=type_saga
	from CFG..EQT_SAGA
	where numero  = @va_Eqt_in

	if @@rowcount<>1 return XDC_NOK
    end
    return XDC_OK     
go  
