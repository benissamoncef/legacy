/*E*/
/*Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAV * FICHIER xzav01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzav01sp.h" */

/* static char *version = "@(#)xzav01.c    1.1 04/09/07 : xzav01" ; */

/* declaration de fonctions internes */

/* definitions de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un eqt du  domaine eclairage dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAV04' and type = 'P')
	drop procedure XZAV04
go

create procedure XZAV04
    @va_horodate_in           datetime,
    @va_type_in        		  tinyint,
    @va_libelle_in         	  char(200),
    @va_alerte_in			  int,
    @va_numero_fmc_in		  int,
    @va_cle_fmc_in			  tinyint,
    @va_acq_alerte_in		  tinyint,
    @va_echangeur_in	  int,
    @va_capteur1_in			 tinyint,
    @va_contexte_in	tinyint
    as 

/* 
*
* SP    XZAV04SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* 
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion dans la table GAB_TRA
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG    06/06/18    : Creation GABEB (DEM/1283)
------------------------------------------------------*/
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_horodate_in             = null or 
       @va_type_in          = null or 
       @va_libelle_in         = null 

        return XDC_ARG_INV
   
              /*A
                   ** Insertion dans la table GAB_TRA
                    */
            
                        insert into EXP..GAB_TRA ( horodate,
                                                   type,
                                                   libelle,
                                                   alerte,
                                                   numero_fmc,
                                                   cle_fmc,
                                                   acq_alerte,
                                                   echangeur,
                                                   capteur1,
                                                   contexte)
                                          values ( @va_horodate_in,
                                          		    @va_type_in,
                                          		    @va_libelle_in,
                                          		    @va_alerte_in,
                                          		    @va_numero_fmc_in,
                                          		    @va_cle_fmc_in,
							    @va_acq_alerte_in,
							    @va_echangeur_in,
							    @va_capteur1_in,
							    @va_contexte_in	)
												                  
						
								/*A
								** Si erreur d'insertion
								*/

								if @@rowcount <> 1
								begin

									/*A
									** Affectation du parametre de sortie Resultat a XDC_MODIF
									*/

									return XDC_NOK
								end   
     
    return XDC_OK
         
go  
