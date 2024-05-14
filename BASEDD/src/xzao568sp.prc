/*E*/
/* Fichier : $Id: xzao568sp.prc,v 1.2 2018/01/09 11:59:44 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/01/09 11:59:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao568sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
* LCL		05/07/17 : modif PRA
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao568sp.h" */

/* static char *version = "@(#)xzao568.c	1.1 23/18/94 : xzao568" ; */

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

if exists (select * from sysobjects where name = 'XZAO568' and type = 'P')
	drop procedure XZAO568
go

	create procedure XZAO568
	   @va_Basedd_in         	     char(3),
	   @va_Numero_in				 smallint,
	   @va_Station1_in			 	 smallint,
	   @va_Station2_in			 	 smallint,
	   @va_Station3_in				 smallint,
	   @va_Operateur1_in			 tinyint,
	   @va_Operateur2_in			 tinyint,
	   @va_Operateur3_in			 tinyint,
	   @va_Inhibition_in			 bit,
	   @va_Resultat_out		int output
	   
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*				    va_Numero_in
*			 		va_Station1_in
*					va_Station2_in
*					va_Station3_in
*					va_Operateur1_in
*					va_Operateur2_in
*					va_Operateur3_in
*				    va_Inhibition_in
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
* Mise � jour des donn�es dans la table ZON_REG
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
declare	@vl_Site	char(10)

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
                        ** Modification de l equipement dans la table REG_VIRTUELLE
                        */

                        update CFG..REG_VIRTUELLE
                        set    station1 = @va_Station1_in,
							   station2 = @va_Station2_in,
							   station3 = @va_Station3_in,
							   operateur1 = @va_Operateur1_in,
							   operateur2 = @va_Operateur2_in,
							   operateur3 = @va_Operateur3_in,
							   inhibition = @va_Inhibition_in  
	
                        where  numero = @va_Numero_in

                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						begin

							/*A
							** Insertion dans la table REG_VIRTUELLE
							*/

				
							insert into CFG..REG_VIRTUELLE (numero,
													   station1,
													   station2,
													   station3,
													   operateur1,
													   operateur2,
													   operateur3,
													   inhibition)
											  values ( @va_Numero_in,
													   @va_Station1_in,
													   @va_Station2_in,
													   @va_Station3_in,
													   @va_Operateur1_in,
													   @va_Operateur2_in,
													   @va_Operateur3_in,
													   @va_Inhibition_in)				
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
                        ** Modification de l equipement dans la table REG_VIRTUELLE
                        */

                        update CFT..REG_VIRTUELLE
                        set    station1 = @va_Station1_in,
							   station2 = @va_Station2_in,
							   station3 = @va_Station3_in,
							   operateur1 = @va_Operateur1_in,
							   operateur2 = @va_Operateur2_in,
							   operateur3 = @va_Operateur3_in,
							   inhibition = @va_Inhibition_in	  
	
                        where  numero = @va_Numero_in

                        /*A
                        ** Si pas d'enregistrement
                        */

						if @@rowcount <> 1
						begin

							/*A
							** Insertion dans la table REG_VIRTUELLE
							*/

				
							insert into CFT..REG_VIRTUELLE (numero,
													   station1,
													   station2,
													   station3,
													   operateur1,
													   operateur2,
													   operateur3,
													   inhibition)
											  values ( @va_Numero_in,
													   @va_Station1_in,
													   @va_Station2_in,
													   @va_Station3_in,
													   @va_Operateur1_in,
													   @va_Operateur2_in,
													   @va_Operateur3_in,
													   @va_Inhibition_in)				
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
