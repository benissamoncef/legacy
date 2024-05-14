/*E*/
/*Fichier : $Id: xzai501sp.prc,v 1.1 2005/06/13 16:52:45 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2005/06/13 16:52:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai501sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot	12 Fev 2004	: Creation
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des messages a diffuser provenant de DATEX 
*  et qui ne sont pas li�s a une FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAI501' and type = 'P')
	drop procedure XZAI501
go

create procedure XZAI501
  as 

/* 
*
* SP	XZAI501SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XZAITListe
*
* VALEUR RENDUE :
*
* Retourne la liste des messages a diffuser ( intitule evt+numero
* de diffusions horodate premiere diffusion +flag "a supprimer")
* provenant de DATEX et qui ne sont pas li�s a une FMC
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
   
   /*A
   ** Interrogation de la table EXP..TFM_DTX
   */   
      

   select  code_partenaire, reference_datex, texte_message_1, texte_message_2, nombre_diffusions, heure_diffusion, supprime from EXP..TFM_DTX

return ( XDC_OK )

go  
