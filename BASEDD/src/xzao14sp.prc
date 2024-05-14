/*E*/
/*Fichier : $Id: xzao14sp.prc,v 1.6 2010/12/10 18:07:27 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2010/12/10 18:07:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao14sp.h >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO14SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		Camera
* XDY_Octet		Position
* XDY_Autoroute		Autoroute
* XDY_PR		PR1_In
* XDY_PR		PR2_In
* XDY_Sens		Sens_In
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* Memorise un prepositionnement correspondant a une zone geographique
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Connection prealable a la base du CI
*
* FONCTION :
*
* Insert dans les tables EQT_CAM_POS (POSITION) et EQT_CAM (CAMERA)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* JPL		10/12/2010 : Degroupee; ajout pertinence dans la table des positions / zones (DEM 948)  1.5
* JPL		10/12/2010 : Ne plus ecrire une position dans la table des cameras (DEM 954)  1.6
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao14sp.h" */

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* static char *version = "$Id: xzao14sp.prc,v 1.6 2010/12/10 18:07:27 gesconf Exp $ : xzao14sp" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

use PRC
go

if exists (select * from sysobjects where name = 'XZAO14' and type = 'P')
	drop procedure XZAO14
go


create procedure XZAO14
	@Camera_In		smallint,
	@Position_In		tinyint,
	@Autoroute_In		tinyint,
	@PR1_In			int,
	@PR2_In			int,
	@Sens_In		tinyint
as
   
   /* Declaration des variables locales */
   
	declare	@Position_Cam	tinyint,
		@Pertinence	tinyint 
   
   /* Test des parametres d'entree :
      Si un des parametres d'entree est egal a null alors XDC_NOK   */
      
   if @Camera_In = null
   or @Position_In = null
   or @Autoroute_In = null
   or @PR1_In = null
   or @PR2_In = null
   or @Sens_In = null

      return XDC_NOK

   else   

   begin
      
      /* Test de l'existence de cet enregistrement */
      
      select @Position_Cam = numero from CFG..EQT_CAM 
      
      if @Position_Cam = null
      begin
          
         /* Insertion de la zone couverte */

         /* La valeur de pertinence est fixee par defaut */
         select @Pertinence = 10

         insert into CFG..EQT_CAM_POS
                        (camera,
                         numero,
                         autoroute,
                         PR_debut,
                         PR_fin,
                         sens,
                         pertinence)
         values (@Camera_In, @Position_In, @Autoroute_In, @PR1_In, @PR2_In, @Sens_In, @Pertinence)
      
      end
         
      return XDC_OK
   end
         
go  
