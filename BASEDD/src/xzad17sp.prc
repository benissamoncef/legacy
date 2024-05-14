/*E*/
/*  Fichier : $Id: xzad17sp.prc,v 1.3 2015/02/02 16:21:31 gesconf Exp $    Release : $Revision: 1.3 $       Date : $Date: 2015/02/02 16:21:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad17sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fournit la liste des PMVA affichant une destination + tdp
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	23/06/05	creation
* VR	26/05/12	: Ajout DTP PMVA (DEM/1014 PMA)
* PNI	28/01/2015	: remplacement de dest <> null par dest >0 DEM1106 1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* fournit la liste des PMVA affichant une destination + tdp
* 
* Sequence d'appel
* SP	XZAD17_Liste_PMVA_Tdp
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
if exists (select * from sysobjects where name = 'XZAD17' and type = 'P')
	drop procedure XZAD17
go	

create procedure XZAD17
	
as
	declare  @vl_Retour int


	select @vl_Retour = XDC_OK


	/*A recherche des franchissement sans date de fin */
#ifdef HIST
		select 
    			APMV.actio,
    			PMVA,
    			premiere_ligne,
    			deuxieme_ligne,
    			troisieme_ligne,
    			quatrieme_ligne,
    			cinquieme_ligne,
    			premiere_ligne_alternat,
    			deuxieme_ligne_alternat,
    			troisieme_ligne_alternat,
    			quatrieme_ligne_alternat,
    			cinquieme_ligne_alternat,
    			clignotement1,
    			clignotement2,
    			clignotement3,
    			clignotement4,
    			clignotement5,
    			APMV.sit,
    			dest_un_proche,
    			dest_un_lointaine,
    			dest_deux_proche,
    			dest_deux_lointaine,
    			depass_seuil	  
		
		from 	HIS..ACT_PMA APMV, HIS..ACT_GEN GEN
		where	
			(dest_un_proche >0 or dest_un_lointaine >0 or dest_deux_proche >0 or dest_deux_lointaine >0) and
			APMV.actio=GEN.numero and
			APMV.sit=GEN.sit and
			heure_succes <> null and
			heure_fin = null and
			heure_echec = null and
			heure_lancement_fin = null
#else
		select 
    			APMV.actio,
    			PMVA,
    			premiere_ligne,
    			deuxieme_ligne,
    			troisieme_ligne,
    			quatrieme_ligne,
    			cinquieme_ligne,
    			premiere_ligne_alternat,
    			deuxieme_ligne_alternat,
    			troisieme_ligne_alternat,
    			quatrieme_ligne_alternat,
    			cinquieme_ligne_alternat,
    			clignotement1,
    			clignotement2,
    			clignotement3,
    			clignotement4,
    			clignotement5,
    			APMV.sit,
    			dest_un_proche,
    			dest_un_lointaine,
    			dest_deux_proche,
    			dest_deux_lointaine,
    			depass_seuil	  
		
		from 	EXP..ACT_PMA APMV, EXP..ACT_GEN GEN
		where	
			(dest_un_proche >0 or dest_un_lointaine >0 or dest_deux_proche >0 or dest_deux_lointaine >0) and
			APMV.actio=GEN.numero and
			APMV.sit=GEN.sit and
			heure_succes <> null and
			heure_fin = null and
			heure_echec = null and
			heure_lancement_fin = null
#endif
			
	return @vl_Retour
go






