/*E*/
/* Fichier : $Id: xzao145sp.prc,v 1.1 2019/05/14 11:25:21 pc2dpdy Exp $        $Revision: 1.1 $        $Date: 2019/05/14 11:25:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao144sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	07/05/19 : Creation GAPLA DEM1334 1.0
------------------------------------------------------*/

/* fichiers inclus */

/*
#include "xzao145sp.h" 
*/

/* static char *version = "@(#)xzao145.c	1.1 20/09/94 : xzao145" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lecture config PAU dans EQT_PAU
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO145' and type = 'P')
	drop procedure XZAO145
go


create procedure XZAO145
   @va_NumEqt_in		smallint,
   @va_CodeAntenne_out		char(3) output,
   @va_CodePAU_out		char(3) output,
   @va_Type_out			tinyint output,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO144SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_NumEqt_in
*
* PARAMETRES EN SORTIE :
*
* XDY_CodeEqt		va_CodeAntenne_out
* XDY_CodeEqt		va_CodePAU_out
* XDY_Octet		va_Type_out
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PAU en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table EQT_PAU (EQT_PAU)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Site 	tinyint

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
         /*A
         **  Travail dans la base CFG
         */   
	 if (@va_NumEqt_in > 0)
	 begin
         select @va_CodeAntenne_out = code_antenne,
	 	@va_CodePAU_out = code_PAU,
		@va_Type_out = type_PAU
	from CFG..EQT_PAU
	where numero = @va_NumEqt_in
           	end
		else
		begin
         select numero
	from CFG..EQT_PAU
		where type_PAU != 1
		end
                  
             /*A
              ** Affectation du parametre de sortie Resultat a XDC_AJOUT
              */
         
             select @va_Resultat_out = XDC_LU
                     
   return XDC_OK
         
go  
