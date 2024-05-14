/*E*/
/*Fichier : $Id: xzaa01sp.prc,v 1.13 2021/09/07 12:43:49 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2021/09/07 12:43:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* gaborit	29 dec 1994	: modif integration (1.2)
* C.P.	07/01/98	Correction gestion multi-sites dans les procs (1.3)
* C.T.  07/01/98	: Ajout des sites suppl (1532) (1.4)
* L.C.  09/07/07        : Suppression DY (1.5)
* JMG           02/10/09        : secto DEM887 1.6
* PNI	09/09/16	Mise en place d'un begin tran v1.7 DEM1179
*  JMG           03/03/17 : regio DEM1220 1.8
* LCL		29/06/17 : PRA  1.9
* JMG		07/05/19 : GAPLA DEM1334 1.10
* LCL		19/11/19 : MOVIS DEM-SAE93 1.11
* LCL	31/08/21 : Correction  pour PF de dev NomSiteLocal optionnel DEM-SAE93 DEM-SAE311 1.12
* LCL	06/09/21 : Fix DEM-SAE93 DEM-SAE311 1.13
* LCL	07/09/21 : Fix DEM-SAE93 DEM-SAE311 1.14
* ABE 19/09/22 : Surcharger par la XZAA010 Consignation bouchon DEM-SAE412
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appel la xzaa010 pour Ajouter une alerte a la liste des alertes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAA;01
   @va_Horodate_in	datetime = null,
   @va_TypeEqt_in	tinyint = null,
   @va_NumEqt_in	smallint = null,
   @va_Type_in		tinyint = null,
   @va_Texte_in		char(100),
   @va_NomSiteLocal_in	char(2) = null
   
   as 

/* 
*
* SP	XZAA01SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_NumEqt_in
* XDY_TypeAlerte	va_Type_in
* XDY_TexteAlerte	va_Texte_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Ajoute une alerte a la liste des alertes
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
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_alerte 		int,
	@vl_Status int
       
   exec @vl_Status = PRC..XZAA010   @va_Horodate_in,
                  @va_TypeEqt_in,
                  @va_NumEqt_in,
                  @va_Type_in,
                  @va_Texte_in,
                  @va_NomSiteLocal_in,
                  @vl_alerte output
     
   return ( @vl_Status )
         
go  
