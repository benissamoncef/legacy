/*E*/
/*  Fichier : $Id: xzae300sp.prc,v 1.1 2014/10/01 09:32:53 gesconf Exp $      Release : $Revision: 1.1 $      Date : $Date: 2014/10/01 09:32:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae300sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* localisation des voies
* Remarque :                                                                   
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   25/11/11        creation 
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE300
* 
* Arguments en entree
*  datetime datedenut   
*  
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*
* Liste les FMC pour EVEREST
* 
* Code retour
* XDC_OK
* XDC_NOK	:
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* UNIQUEMENT SUR HIS
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAE300
go

create procedure XZAE300
        @va_datedebut	datetime = null 
as

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos en base                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/
	declare @vl_Cloture datetime



	select G.numero,G.cle,G.debut,G.fin,autoroute,PR,sens,G.cause,G.cle_cause,A.cond_meteo ,A.cond_visibilite ,A.cond_etat_chaus ,A.cond_chantier, A.nombre_de_vl, A.nombre_de_pl ,A.nombre_de_remorques ,A.nombre_de_car ,A.nombre_de_moto,VR,VM2 ,VM1,VL ,BAU, blesses_legers, blesses_graves, morts
	from HIS..FMC_GEN G,HIS..FMC_HIS H,HIS..FMC_ACC A
	where G.cloture!=null and 
		G.debut > @va_datedebut and
		G.numero=H.numero and G.numero=A.numero and
		G.cle=H.cle and G.cle=A.cle and
		H.horodate_validation=A.validation
	group by G.numero,G.cle
	having 
		G.cloture!=null and 
		G.debut > @va_datedebut and 
		G.numero=H.numero and G.numero=A.numero and
		G.cle=H.cle and G.cle=A.cle and
		H.horodate_validation=A.validation and 
		horodate_validation=max(horodate_validation)
	order by numero,cle

return (XDC_OK)

go
