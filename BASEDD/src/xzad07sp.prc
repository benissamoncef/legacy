/*E*/
/*  Fichier : $Id: xzad07sp.prc,v 1.4 2015/02/02 16:21:31 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2015/02/02 16:21:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fournit la liste des PMV affichant une destination + tdp
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	24/02/97	: Creation	(dem/tdp) 	(V 1.1)
* C.P.	10/06/98	: Separation EXP et HIS 	(V 1.2)
* JMG	13/01/12	: tdp etendu DEM/1014		1.3
* PNI	28/01/2015	: remplacement de dest <> null par dest >0 DEM1106 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* fournit la liste des PMV affichant une destination + tdp
* 
* Sequence d'appel
* SP	XZAD07_Liste_PMV_Tdp
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 numero inexistante
* 
* Conditions d'utilisation
* 
* Fonction
* Consultation de la table ACT_PMV et ACT_GEN
-------------------------------------------------------*/

use PRC
go

drop procedure XZAD07
go

create procedure XZAD07
	
as
	declare  @vl_Retour int


	select @vl_Retour = XDC_OK


	/*A recherche des franchissement sans date de fin */
#ifdef HIST
		select 
    			APMV.actio,
    			PMV,
    			premiere_ligne,
    			deuxieme_ligne,
    			troisieme_ligne,
    			premiere_ligne_alternat,
    			deuxieme_ligne_alternat,
    			troisieme_ligne_alternat,
    			pictogramme,
    			bandeau,
    			clignotement1,
    			clignotement2,
    			clignotement3,
   			clignotement_picto,
    			APMV.sit,
    			direction_ligne, 
    			direction_ligne_alternat,
    			destination,
    			destination_alternat,
    			depass_seuil	  ,
			destination_noeud
		
		from 	HIS..ACT_PMV APMV, HIS..ACT_GEN GEN
		where	
			(destination >0 or destination_alternat >0) and
			APMV.actio=GEN.numero and
			APMV.sit=GEN.sit and
			heure_succes <> null and
			heure_fin = null and
			heure_echec = null and
			heure_lancement_fin = null
#else
		select 
    			APMV.actio,
    			PMV,
    			premiere_ligne,
    			deuxieme_ligne,
    			troisieme_ligne,
    			premiere_ligne_alternat,
    			deuxieme_ligne_alternat,
    			troisieme_ligne_alternat,
    			pictogramme,
    			bandeau,
    			clignotement1,
    			clignotement2,
    			clignotement3,
   			clignotement_picto,
    			APMV.sit,
    			direction_ligne, 
    			direction_ligne_alternat,
    			destination,
    			destination_alternat,
    			depass_seuil	  ,
			destination_noeud
		
		from 	EXP..ACT_PMV APMV, EXP..ACT_GEN GEN
		where	
			(destination >0 or destination_alternat >0) and
			APMV.actio=GEN.numero and
			APMV.sit=GEN.sit and
			heure_succes <> null and
			heure_fin = null and
			heure_echec = null and
			heure_lancement_fin = null
#endif
			
	return @vl_Retour
go






