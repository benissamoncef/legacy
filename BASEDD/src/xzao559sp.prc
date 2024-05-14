/*E*/
/* Fichier : $Id: xzao559sp.prc,v 1.3 2018/07/11 11:20:56 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 11:20:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao559sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
* LCL		05/07/17 : Modif PRA
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao559sp.h" */

/* static char *version = "@(#)xzao559.c	1.1 23/18/94 : xzao559" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO559' and type = 'P')
	drop procedure XZAO559
go
	create procedure XZAO559
	   @va_Basedd_in         	     char(3),
	   @va_NumeroScen_in			 smallint,
	   @va_NumeroPRV_in			 	 smallint,
	   @va_PRVCritique_in			 bit,
	   @va_Commande_in			  	 smallint
	
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*	   				va_NumeroScen_in
*	  				va_NumeroPRV_in
*	   				va_PRVCritique_in
*	   				va_Commande_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Mise a jour des donnees dans la table ZON_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012	: Creation (DEM/1016)
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
	@vl_Site 	char(10)

     /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null
	or @va_NumeroScen_in	     = null
	or @va_NumeroPRV_in		     = null
	or @va_PRVCritique_in	     = null
	or @va_Commande_in		     = null
	  	return XDC_ARG_INV
   
    else   
    begin
 
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin

            /*A
            **  Travail dans la base CFG
            */   
      
				if @va_Basedd_in = XDC_BASE_CFG
            
					begin
                        /*A
                        ** Modification de l equipement dans la table REG_SCE_CMD
                        */

			if (@va_Commande_in>0)
			begin
                        update CFG..REG_SCE_CMD
                        set    commande = @va_Commande_in,
			   prv_critique = @va_PRVCritique_in
                        where  prv = @va_NumeroPRV_in and
			   scenario = @va_NumeroScen_in
			end
			else
			begin
				delete CFG..REG_SCE_CMD
				where  prv = @va_NumeroPRV_in and
				scenario = @va_NumeroScen_in
				return XDC_OK
			end
                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						begin

							/*A
							** Insertion dans la table REG_SCE_CMD
							*/

				
							insert into CFG..REG_SCE_CMD (prv,
													   scenario,
													   commande,
													   prv_critique)
											  values ( @va_NumeroPRV_in,
													   @va_NumeroScen_in,
													   @va_Commande_in,
													   @va_PRVCritique_in)				
								/*A
								** Si erreur d'insertion
								*/

								if @@rowcount <> 1
								begin

									/*A
									** Affectation du parametre de sortie Resultat a XDC_MODIF
									*/


									return XDC_OK
								end  
						end
					end
#ifdef CI      
            else

            /*A
            **  Travail dans la base CFT
            */   

				if @va_Basedd_in = XDC_BASE_CFT
            
					begin
                        /*A
                        ** Modification de l equipement dans la table REG_SCE_CMD
                        */

			if  @va_Commande_in > 0
			begin
                        	update CFT..REG_SCE_CMD
                        	set    commande = @va_Commande_in,
				   prv_critique = @va_PRVCritique_in
                        	where  prv = @va_NumeroPRV_in and
				   scenario = @va_NumeroScen_in
			end
			else
			begin
				delete CFG..REG_SCE_CMD
				where  prv = @va_NumeroPRV_in and
				scenario = @va_NumeroScen_in
				return XDC_OK
			end

                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						begin

							/*A
							** Insertion dans la table REG_SCE_CMD
							*/

				
							insert into CFT..REG_SCE_CMD (prv,
													   scenario,
													   commande,
													   prv_critique)
											  values ( @va_NumeroPRV_in,
													   @va_NumeroScen_in,
													   @va_Commande_in,
													   @va_PRVCritique_in)				
								/*A
								** Si erreur d'insertion
								*/

								if @@rowcount <> 1
								begin

									/*A
									** Affectation du parametre de sortie Resultat a XDC_MODIF
									*/


									return XDC_OK
								end   
				end
			end
#endif
      
	    end
	   else   
    	begin      
        /*A
        ** Verification que le serveur est bien celui du PR
        */
      
    /* PRA
    if @@servername = XDC_SQLPR
    */
    if @vl_Site = XDC_DP
        begin
	    /*A
        ** Modification de l equipement dans la table REG_SCE_CMD
        */

		if (@va_Commande_in>0)
			begin
                update CFG..REG_SCE_CMD
                set    commande = @va_Commande_in,
					   prv_critique = @va_PRVCritique_in
                where  prv = @va_NumeroPRV_in and
					   scenario = @va_NumeroScen_in
			end
		else
			begin
				delete CFG..REG_SCE_CMD
				where  prv = @va_NumeroPRV_in and
					   scenario = @va_NumeroScen_in
				return XDC_OK
			end
            /*A
            ** Si pas d'enregistrement
            */
			if @@rowcount <> 1
				begin
				/*A
				** Insertion dans la table REG_SCE_CMD
				*/	
					insert into CFG..REG_SCE_CMD (prv,
													   scenario,
													   commande,
													   prv_critique)
											  values ( @va_NumeroPRV_in,
													   @va_NumeroScen_in,
													   @va_Commande_in,
													   @va_PRVCritique_in)				
					/*A
					** Si erreur d'insertion
					*/
					if @@rowcount <> 1
						begin	
						/*A
						** Affectation du parametre de sortie Resultat a XDC_MODIF
						*/
							return XDC_OK
						end  
				end
		end
		end	    
end         
     
    return XDC_OK
         
go  
