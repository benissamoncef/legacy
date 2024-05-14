/*E*/
/*Fichier : @(#)xzai03sp.prc	1.6      Release : 1.6        Date : 01/12/12
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* gaborit	02 dec 1994	: modif non conformite 1.2
* gaborit	22 dec 1994	: modif reveil
* torregrossa	11 avr 1995	: pas de declenchement de reveil 
*                                 quand l'archivage se fait au CI (1.4)
* niepceron	18 Oct 1996	: Ajout du niveau (dem/1248) 1.5
* JPL		09/01/2012 : Changement libelle "TRAFIC FM" en "RVA" (DEM 1009)  1.6
* LCL		04/07/17	: Modif PRA 1.8
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Archive le message et les commentaires de l'operateur sur le message et diffuse le message au CI
*  (cf XZAP16)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAI;03
   @va_NumFMC_in		int = null,
   @va_CleFMC_in		tinyint = null,
   @va_Confidentiel_in		bit,
   @va_Niveau_in		tinyint = null,
   @va_Horodate_in		datetime
   as 

/* 
*
* SP	XZAI03SP
*
* PARAMETRES EN ENTREE :
*
* XDY_FMC		va_NumFMC_in
* XDY_Booleen		va_Confidentiel_in
* XDY_Octet		va_Niveau_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Horodate		va_Horodate_out
*
* VALEUR RENDUE :
*
* Archive le message et les commentaires de l'operateur sur le message et diffuse le message au CI
* (cf XZAP16)
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION : appel apres xzai08
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare @vl_FMC		int,
		@vl_Site	tinyint
       
	/*A
	** Test des parametres d'entree :
	**   Si un des parametres d'entree est egal a null alors XDC_NOK   
	*/
    if @va_NumFMC_in = null or @va_CleFMC_in = null
		return ( XDC_ARG_INV )
	
	else   
   	/*A
   	** Si les parametres sont non nuls
   	*/
   	begin
      
    	/*A
    	** Travail dans la base EXP
      	*/   
      
		/*A
		** Recherche l'horodate correspondant a la fiche main courante
		*/
			
		select @vl_FMC = numero 
			from EXP..FMC_TFM
				where numero 	 = @va_NumFMC_in
					and cle		 = @va_CleFMC_in
					and horodate     = @va_Horodate_in

		if @vl_FMC != null
		begin

			update EXP..FMC_TFM
		   		set confidentiel = @va_Confidentiel_in, 
		       		niveau	  = @va_Niveau_in
		   			where numero 	= @va_NumFMC_in
		     				and cle	= @va_CleFMC_in 
		     				and horodate = @va_Horodate_in
          

			/* Verification que le site est bien celui du CI ou CA */
			select  @vl_Site = numero
				from CFG..RES_DIS
					where serveur = @@servername and type = XDC_TYPEM_SD

			/* Si le site n'est pas CI ni CA */
			if @@rowcount = 0
				exec PRC..XZAR;03 	@va_NumFMC_in, 
									@va_CleFMC_in, 
									null, 
									"MESSAGE RVA !",
									@vl_Site
        end
      	else
      
         return (XDC_NOK)     		
   end
     
   return ( XDC_OK )
         
go  
