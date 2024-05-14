/*E*/
/* Fichier : $Id: xzao554sp.prc,v 1.4 2018/10/11 12:24:56 devgfi Exp $        Release : $Revision: 1.4 $        Date : $Date: 2018/10/11 12:24:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao554sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		12/01/12 : Creation (DEM/1016)
* LCL		05/07/17 : Modif PRA
* LCL		11/07/18 : correction syntaxe from
* GGY		17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao554sp.h" */

/* static char *version = "@(#)xzao554.c	1.1 23/18/94 : xzao554" ; */

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

if exists (select 1 from sysobjects where name = 'XZAO554' and type = 'P')
	drop procedure XZAO554
go

	create procedure XZAO554
	   @va_Basedd_in         	char(3),
	   @va_Numero_in			smallint,
	   @va_Type_in			 	tinyint,
	   @va_VitesseNominale_in	smallint,
	   @va_Zone_in				smallint,
	   @va_Critique_in			bit,
	   @va_Resultat_out		int output
	   as 

/* 
*
* SP    XZAO554SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Numero_in
*  					va_Type_in
*  					va_VitesseNominale_in
* 					va_Zone_in
*  					va_Critique_in
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
* Mise a jour des donn�es dans la table EQT_PRV
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    12 Jan 2012	: Creation (DEM/1016)
* GGY	26/09/23 	: Ajout district CA (DEM483)
------------------------------------------------------*/
declare
	@vl_Site	char(10)

/*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
	if @va_Basedd_in = null  
	   or @va_Numero_in = null 
	   or @va_Type_in = null 
	   or @va_VitesseNominale_in = null 
	   or @va_Critique_in = null 
	   or @va_Zone_in = null
         return XDC_ARG_INV
	
    select @va_Resultat_out = XDC_LU
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null
	   
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
                        ** Modification de l equipement dans la table EQT_PRV
                        */

                        update CFG..EQT_PRV
                        set    type = @va_Type_in,
							   vitesse_nominale = @va_VitesseNominale_in,
							   zone = @va_Zone_in,
							   critique = @va_Critique_in

                        where  numero = @va_Numero_in

                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						
						begin

							/*A
							** Insertion dans la table EQT_PRV
							*/

				
							insert into CFG..EQT_PRV ( numero,
													   type,
													   vitesse_nominale,
													   zone,
													   critique)
											  values ( @va_Numero_in,
													   @va_Type_in,
													   @va_VitesseNominale_in,
													   @va_Zone_in,
													   @va_Critique_in)				
									/*A
									** Si erreur d'insertion
									*/

									if @@rowcount <> 1
									begin

										/*A
										** Affectation du parametre de sortie Resultat a XDC_MODIF
										*/

										select @va_Resultat_out = XDC_MODIF

										return XDC_NOK
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
                        ** Modification de l equipement dans la table EQT_PRV
                        */

                        update CFT..EQT_PRV
                        set    type = @va_Type_in,
							   vitesse_nominale = @va_VitesseNominale_in,
							   zone = @va_Zone_in,
							   critique = @va_Critique_in

                        where  numero = @va_Numero_in

                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						begin

							/*A
							** Insertion dans la table EQT_PRV
							*/

				
							insert into CFT..EQT_PRV ( numero,
													   type,
													   vitesse_nominale,
													   zone,
													   critique)
											  values ( @va_Numero_in,
													   @va_Type_in,
													   @va_VitesseNominale_in,
													   @va_Zone_in,
													   @va_Critique_in)				
								/*A
								** Si erreur d'insertion
								*/

								if @@rowcount <> 1
								begin

									/*A
									** Affectation du parametre de sortie Resultat a XDC_MODIF
									*/

									select @va_Resultat_out = XDC_MODIF

									return XDC_NOK
								end   
						end
					end
#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
