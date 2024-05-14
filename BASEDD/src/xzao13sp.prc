/*E*/
/*  Fichier : $Id: xzao13sp.prc,v 1.8 2019/09/06 17:07:50 devgtie Exp $      Release : $Revision: 1.8 $        Date : $Date: 2019/09/06 17:07:50 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao13sp.h >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO13SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute		Autoroute
* XDY_PR		PR1_In
* XDY_PR		PR2_In
* XDY_Sens		Sens_In
*
* PARAMETRES EN SORTIE :
*
* XDY_Eqt		ListeCamera
* XDY_Octet		ListePosition	
*
* VALEUR RENDUE :
*
* Retourne les cameras disponibles qui couvrent la zone comprise
* entre PR1 et PR2 et les numeros de prepositionnement correspondants
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
* Select dans les tables EQT_CAM_POS (POSITION), EQT_DSP (INDISPONIBILITE)
* et EQT_CAM (CAMERA)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994 : Creation
* gaborit	10 jan 1995 : modif integration (1.5)
* JPL		28/10/2010 : Degroupee; retourne pertinence; tri par pertinence (DEM 949)  1.6
* JPL		16/02/2011 : Prise en compte des PRs inverses en table EQT_CAM_POS         1.7
* PNI           06/09/2019 : DEM1345 - Ajout d'une icone caméra dans l'IHM FMC             1.8
------------------------------------------------------*/


use PRC
go

if exists (select * from sysobjects where name = 'XZAO13' and type = 'P')
	drop procedure XZAO13
go


create procedure XZAO13
   @va_Autoroute_In	tinyint,
   @va_PR1_In		int,
   @va_PR2_In		int,
   @va_Sens_In		tinyint
   as
   
   declare @vl_PR int
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Autoroute_In = null or @va_PR1_In = null or @va_PR2_In = null or @va_Sens_In = null
      return XDC_ARG_INV
   else
   begin
      /*B echange des PR pour que PR1<PR2 */
      if @va_PR1_In>@va_PR2_In
      begin
         select @vl_PR=@va_PR1_In
         select @va_PR1_In=@va_PR2_In
         select @va_PR2_In=@vl_PR
      end
      
      /*A
      ** Selection des cameras disponibles couvrant la zone entre PR1 et PR2
      ** et le numeros de prepositionnement correspondants                   
      */
    
      select distinct ECP.camera, ECP.numero, ECP.pertinence
         from CFG..EQT_CAM_POS ECP /*, EXP..EQT_DSP ED*/
         where  ECP.autoroute = @va_Autoroute_In
            and ( (ECP.PR_debut <= @va_PR2_In  and  ECP.PR_fin >= @va_PR1_In)
               or (ECP.PR_fin   <= @va_PR2_In  and  ECP.PR_debut >= @va_PR1_In)
                )
            and (
                   ECP.sens      = @va_Sens_In
                or ECP.sens      = XDC_SENS_INCONNU
                or @va_Sens_In   = XDC_SENS_INCONNU
                )
   /*         and ED.equipement = ECP.camera
            and ED.type       = XDC_EQT_CAM
            and ED.dernier    = XDC_OUI
            and ED.majeure   = XDC_NON
            and ED.critique  = XDC_NON
            and ED.HS        = XDC_NON
            and ED.inhibe    = XDC_NON*/
         order by pertinence
             
        
      return XDC_OK
   end
         
go            

