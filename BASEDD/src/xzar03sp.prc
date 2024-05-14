/*E*/
/* Fichier : xzar03sp.prc
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAR * FICHIER xzar03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Base / Postes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* gaborit	20 Dec 1994	: Modif (site)
* torregrossa	14 Mar 1995	: Modif longueur texte reveil (V 1.3)
* guilhou	08 jan 1998	: gestion multi-sites (dem/1536) 1.4
* JMG           02/10/09        : secto DEM887 1.5
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.6
* LCL	05/07/17	: Modif PRA
* LCL	20/11/19	: MOVIS DEM-SAE93 1.8
* LCL 	17/11/20	: Correction PRA DEM-SAE93 1.8
* RGR	08/12/20	: Retrait affichage
* JPL	27/01/22	: Restauration site local (pour SAE-366)  1.12
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute un reveil a la liste de l'operateur concerne
*  ou des operateurs du site concerne.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAR;03
   @va_NumEvt_in		int = null,
   @va_CleEvt_in		tinyint = null,
   @va_PosteOperateur_in	smallint = null,
   @va_TexteReveil_in		char(100),
   @va_Site_in			tinyint = null -- argument optionel
   as 

/* 
*
* SP	XZAR03SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Evt		va_NumEvt_in
* XDY_Mot		va_PosteOperateur_in
* char(100)		va_TexteReveil_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Ajoute un reveil a la liste de l'operateur concerne
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
* Insert REVEIL
* appelle ARRP03
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Site 	tinyint,
   @vl_SiteMOVIS 	tinyint,
   @vl_NomSite 	char(2),
   @vl_Status	int,
   @vl_NomMac   char(10)
          
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_NumEvt_in = null
   or @va_CleEvt_in = null
   or @va_TexteReveil_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   
   begin
      
      /*A
      ** Travail dans la base EXP
      */   
      
	if @va_PosteOperateur_in is not null
	begin
		select @vl_Site = sit from EXP..EQT_DSP
		where type	 = XDC_EQT_MAC
		  and equipement = @va_PosteOperateur_in
		  and dernier    = 1

		select @vl_NomMac = nom from CFG..EQT_GEN
		where type	= XDC_EQT_MAC
		  and numero    = @va_PosteOperateur_in
	end
      
      select @vl_Site=isnull(@vl_Site,@va_Site_in)
      select @va_PosteOperateur_in=isnull(@va_PosteOperateur_in,0)

      select @vl_NomSite = code from CFG..RES_DIS
        where numero    = @vl_Site


      /* MOVIS */
      select  @vl_SiteMOVIS = numero
      from CFG..RES_DIS
      where serveur = @@servername
      
      /*A
      ** Le site est celui du CI
      */

      /* PRA
      if @@servername = XDC_SQLCI and @vl_Site=XDC_CI
      */
		if @vl_SiteMOVIS = XDC_CI and @vl_Site=XDC_CI
		begin
            exec @vl_Status = TASRV_CI...ARRP03 	@va_NumEvt_in, 
							@va_CleEvt_in, 
							@vl_NomMac,
							@vl_NomSite, 
							@va_TexteReveil_in
            if @vl_Status != 0
				return XDC_NOK
		end
		else if @vl_SiteMOVIS = XDC_CA and @vl_Site=XDC_CA
		begin
            exec @vl_Status = TASRV_CA...ARRP03 	@va_NumEvt_in, 
							@va_CleEvt_in, 
							@vl_NomMac,
							@vl_NomSite, 
							@va_TexteReveil_in
            if @vl_Status != 0
				return XDC_NOK
		end

      else
        
      /*A
      ** Le site est celui du VC 
      */
      
      /* PRA
      if @@servername = XDC_SQLVC and @vl_Site=XDC_VC
      */
      if @vl_SiteMOVIS = XDC_VC and @vl_Site=XDC_VC
      begin
               
            exec @vl_Status = TASRV_VC...ARRP03 	@va_NumEvt_in, 
							@va_CleEvt_in, 
							@vl_NomMac,
							@vl_NomSite, 
							@va_TexteReveil_in
			
            if @vl_Status != 0
               
               return XDC_NOK
               
      end
      
      else
        
      /*site DP*/
      /* PRA 
      if @@servername = XDC_SQLDP and @vl_Site=XDC_DP
      */
      if @vl_SiteMOVIS = XDC_DP and @vl_Site=XDC_DP
      begin

		exec @vl_Status = TASRV_DP...ARRP03         @va_NumEvt_in,
								@va_CleEvt_in,
								@vl_NomMac,
								@vl_NomSite,
								@va_TexteReveil_in
		if @vl_Status != 0
			return XDC_NOK
      end

      else
      
      begin
      
            if @vl_Site<>null
            insert into EXP..FMC_REV
            values ( @va_NumEvt_in, @va_CleEvt_in, @va_PosteOperateur_in, 
	    @va_TexteReveil_in, @vl_Site ) 

      end
      return ( XDC_OK )
   
   end
         
go  
