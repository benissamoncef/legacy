/*E*/
 /*Fichier :  $Id: egab_ajuste.c,v 1.2 2018/10/26 15:55:42 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/26 15:55:42 $ ------------------------------------------------------
 ------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
 ------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
 ------------------------------------------------------
 * MODULE TEREG * FICHIER egab_ajuste.c
 ------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < Module de general de ajuste de la regulation >
 *
 ------------------------------------------------------
 * HISTORIQUE :
 *
 * JMG	12/01/18	creation GABEB 1.1
 * JMG	20/10/18 	ajustement reveil  1.2
 ------------------------------------------------------*/

#include "egab_util.h"

#include "egab_ajuste.h"
#include "egab_trace.h"
#include "egab_cfg.h"
#include "xzia.h"
#include "xzae570.h"

#define C_MODE_SUD	XDC_SENS_SUD
#define C_MODE_NORD	XDC_SENS_NORD
#define C_CREATION_ONLY    1
#define C_ALL              2

T_LISTE_CHAINEE *pg_liste_bouchons = NULL;

extern XDY_NomSite vg_NomSite;
extern XDY_NomMachine vg_NomMachine;
extern T_Liste_ECH *pg_ech;
extern T_Liste_CAB *pg_CAB;

int bouchon_bretelle(T_Liste_ECH *, int );
int bouchon_bretelle_debordant(T_Liste_ECH *, int );
int bouchon_bretelle_continuant(T_Liste_ECH *, int );
int bouchon_section_courante(T_Liste_ECH *, int );
void ajuste_recherche_bouchons(T_Liste_ECH *, int, int);
void ajuste_controle_coherence(T_Liste_ECH *, int, int);

void ajuste_analyse_bouchons();
int memoriser_bouchon(XDY_Entier va_numeroQ, XDY_Entier va_cleQ,
			XDY_Entier va_numeroT, XDY_Entier va_cleT,
			XDY_Autoroute va_autorouteQ,
			XDY_Sens va_sensQ,
			XDY_PR va_prQ,
			XDY_PR va_prT, 
			XDY_Octet va_PtCarQ,
			XDY_Octet va_PtCarT, XDY_Entier va_numero_cause,
			XDY_Octet va_cle_cause,
			XDY_Autoroute va_autorouteT,
			XDY_Sens va_sensT,
			XDY_Entier *pa_cr);

int ajuste_analyse(){
  T_Liste_ECH *pl_ech =NULL;
  T_CAB *pl_cab1;
  int vl_mode = C_ALL;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_analyse ");

  pl_ech = pg_ech;

  while (pl_ech !=NULL) {
	pl_cab1 = pl_ech->ech.cab;
	if (pl_cab1!=NULL) {
        if ((pl_cab1->detection1 == EGABC_DETECTION_HS) ||
            (pl_cab1->detection3 == EGABC_DETECTION_HS)){
          XZST_03EcritureTrace(XZSTC_FONCTION, "un cab HS => creation only");
	  vl_mode = C_CREATION_ONLY;
        }

      /*recherche des bouchons en cours*/
      XZST_03EcritureTrace(XZSTC_FONCTION, "recherche bouchons ech %d SUD",pl_ech->ech.echangeur.NumeroEchangeur);
      ajuste_recherche_bouchons(pl_ech, C_MODE_SUD, vl_mode);
    
      /*recherche des bouchons en cours*/
        if ((pl_cab1->detection2 == EGABC_DETECTION_HS) ||
            (pl_cab1->detection4 == EGABC_DETECTION_HS)){
          XZST_03EcritureTrace(XZSTC_FONCTION, "un cab HS => creation only");
          vl_mode = C_CREATION_ONLY;
        }
      XZST_03EcritureTrace(XZSTC_FONCTION, "recherche bouchons ech %d NORD",pl_ech->ech.echangeur.NumeroEchangeur);
      ajuste_recherche_bouchons(pl_ech, C_MODE_NORD, vl_mode);
    }
    pl_ech = pl_ech->suivant;
  }
}

void ajuste_recherche_bouchons(T_Liste_ECH *pa_ech, int va_mode, int va_creation_only) {
  XDY_Horodate vl_horodate;
  int vl_JourSemaine_Msg;
  T_LISTE_CHAINEE *pl_liste_evts;
  T_LISTE_CHAINEE *pl_free;

  if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
                          (int *) &vl_JourSemaine_Msg, (double *) &vl_horodate ) != XDC_OK )
  {
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
	 return;
         }

  /*purge des bouchons du cycle precedent*/
  pl_liste_evts = pg_liste_bouchons;

  for (pl_liste_evts = pg_liste_bouchons; pl_liste_evts !=NULL; free(pl_free)) {
    pl_free= pl_liste_evts;
    pl_liste_evts = pl_liste_evts->suivant;
  }

  pa_ech->ech.bouchon = NULL;

  pg_liste_bouchons=NULL;
  XZAE570_Bouchons_Proches(vl_horodate,
  			pa_ech->ech.echangeur.NumeroAutoroute,
			pa_ech->ech.echangeur.PR,
			va_mode == C_MODE_SUD ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
			pa_ech->ech.echangeur.NumeroEchangeur,
			(XDY_FonctionInt)memoriser_bouchon);

    /*coherence avec les bouchons terrain*/
    ajuste_controle_coherence(pa_ech, va_mode, va_creation_only);
}



int memoriser_bouchon(XDY_Entier va_numeroQ, XDY_Entier va_cleQ,
			XDY_Entier va_numeroT, XDY_Entier va_cleT,
			XDY_Autoroute va_autorouteQ,
			XDY_Sens va_sensQ,
			XDY_PR va_prQ,
			XDY_PR va_prT, 
			XDY_Octet va_PtCarQ,
			XDY_Octet va_PtCarT,
			XDY_Entier va_numero_cause, XDY_Octet va_cle_cause,
			XDY_Autoroute va_autorouteT,
			XDY_Sens va_sensT,
			XDY_Entier *pa_cr)
{
T_Bouchon pl_evenement;
T_LISTE_CHAINEE *pl_liste_evts;

  XZST_03EcritureTrace(XZSTC_FONCTION,
                  "IN : memoriser_bouchon %d %d", va_numeroQ, va_cleQ);
  XZST_03EcritureTrace(XZSTC_FONCTION,
  	"tete %d/%d/%d - queue %d/%d/%d",
	va_autorouteT, va_prT, va_sensT,
	va_autorouteQ, va_prQ, va_sensQ);

  pl_evenement.numeroQ = va_numeroQ;
  pl_evenement.cleQ = va_cleQ;
  pl_evenement.numeroT = va_numeroT;
  pl_evenement.cleT = va_cleT;
  pl_evenement.autorouteQ = va_autorouteQ;
  pl_evenement.autorouteT = va_autorouteT;
  pl_evenement.prQ = va_prQ;
  pl_evenement.prT = va_prT;
  pl_evenement.sensQ = va_sensQ;
  pl_evenement.sensT = va_sensT;
  pl_evenement.ptCarQ = va_PtCarQ;
  pl_evenement.ptCarT = va_PtCarT;
  pl_evenement.numero_cause = va_numero_cause;
  pl_evenement.cle_cause = va_cle_cause;
  pl_liste_evts = malloc(sizeof(T_LISTE_CHAINEE));
                pl_liste_evts->evt = pl_evenement;
                pl_liste_evts->suivant = pg_liste_bouchons;
  pg_liste_bouchons = pl_liste_evts;

 *pa_cr = XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : memoriser_bouchon");
   return XDC_OK;
}


void ajuste_controle_coherence(T_Liste_ECH *pa_ech, int va_mode, int va_creation_only) {
  int vl_detection;
  T_Bouchon *vl_bouchon; 
  char vl_texte[100];

  if (va_mode==C_MODE_SUD)
    vl_detection = pa_ech->ech.detection_sud;
  else
    vl_detection = pa_ech->ech.detection_nord;
  XZST_03EcritureTrace(XZSTC_FONCTION, "ajuste_controle_coherence :detection %d // va_mode %d",
  				vl_detection, va_mode);	

  switch (vl_detection) {
   case (C_PAS_BOUCHON) : 
	XZST_03EcritureTrace(XZSTC_FONCTION, "cas 1 : pas de bouchon en bretelle");
	if (pg_liste_bouchons==NULL) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 1 : pas de bouchon en cours => pas d action");
	  pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	  return;
	}
	if (bouchon_bretelle(pa_ech, va_mode)==XDC_OK) {
	  vl_bouchon = pa_ech->ech.bouchon;
	  if ((va_creation_only != C_CREATION_ONLY) &&
	      (egab_echangeur_ferme(pa_ech->ech.echangeur.NumeroEchangeur, va_mode)==XDC_NON)){	
	    XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 2 : mise en fin du bouchon bretelle existant %d/%d",
	  		vl_bouchon->numeroQ,
			vl_bouchon->cleQ);
	    pa_ech->ech.contexte = C_CONTEXTE_MODIF;
	    valide_termine(pa_ech, va_mode, *vl_bouchon);
	    return;
	  }
	  else {
	    XZST_03EcritureTrace(XZSTC_FONCTION, "creation only => ne rien faire");
	    pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	    return;
	  }
	}
	if (bouchon_bretelle_debordant(pa_ech, va_mode)==XDC_OK) {
	  vl_bouchon = pa_ech->ech.bouchon;
	  if ((va_creation_only != C_CREATION_ONLY) &&
	      (egab_echangeur_ferme(pa_ech->ech.echangeur.NumeroEchangeur, va_mode)==XDC_NON)){	
	    XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 3 : modif bouchon remettre la Q en bretelle %d/%d (aut tete %d)",
	  		vl_bouchon->numeroQ,
			vl_bouchon->cleQ,
			vl_bouchon->autorouteT);
	    valide_deplace(pa_ech, va_mode, *vl_bouchon,
		vl_bouchon->autorouteT,
		vl_bouchon->sensT,
		vl_bouchon->prT,
		vl_bouchon->autorouteQ,
		(va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
		vl_bouchon->prQ,
		pa_ech->ech.echangeur.NumeroEchangeur,
		vl_bouchon->ptCarT);
            pa_ech->ech.contexte = C_CONTEXTE_MODIF;
	    return;  
	  }
	  else {
	    XZST_03EcritureTrace(XZSTC_FONCTION, "creation only => ne rien faire");
	    pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	    return;
	  }
	}
	else if (bouchon_bretelle_continuant(pa_ech, va_mode)==XDC_OK) {
	    vl_bouchon = pa_ech->ech.bouchon;
	    if (egab_echangeur_ferme(pa_ech->ech.echangeur.NumeroEchangeur, va_mode)==XDC_NON) {
	    	XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 4 : reveil evolution bouchon %d/%d",
	    		vl_bouchon->numeroQ,
			vl_bouchon->cleQ);
	    	sprintf(vl_texte, "Pas de bouchon detecte en bretelle");
	    	genererReveil(vl_bouchon->numeroQ, vl_bouchon->cleQ, vl_texte,0,
					pa_ech->ech.echangeur.NumeroEchangeur,
					pa_ech->ech.cab->config.NumeroCAP);
	    	pa_ech->ech.contexte = C_CONTEXTE_REVEIL;
	    }
	    return;
        }
	else if (bouchon_section_courante(pa_ech, va_mode)==XDC_OK) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 5 : bouchon en SC => pas d action");
	  pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	  return;
	}
   	break;

    case (C_BOUCHON_BRETELLE):
	XZST_03EcritureTrace(XZSTC_FONCTION, "cas 2 : bouchon en bretelle");
	if (pg_liste_bouchons==NULL) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 1 : pas de bouchon en cours => creation FMC T/Q bretelle");
	  pa_ech->ech.contexte = C_CONTEXTE_CREATION;
	  valide_creation(pa_ech, 
		va_mode,
		(va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
	  	(va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD);
	  return;
	}
	if (bouchon_bretelle(pa_ech, va_mode)==XDC_OK) {
	  vl_bouchon = pa_ech->ech.bouchon;
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 2 : bouchon bretelle existant %d/%d => pas d action",
	  		vl_bouchon->numeroQ,
			vl_bouchon->cleQ);
	  pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	  return;
        }
	else if (bouchon_bretelle_debordant(pa_ech, va_mode)==XDC_OK) {
	  vl_bouchon = pa_ech->ech.bouchon;
	  if (va_creation_only != C_CREATION_ONLY) {
	    XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 3 : modif bouchon remettre la Q en bretelle %d/%d",
	  		vl_bouchon->numeroQ,
			vl_bouchon->cleQ);
	    valide_deplace(pa_ech, va_mode, *vl_bouchon,
		vl_bouchon->autorouteT,
		vl_bouchon->sensT,
		vl_bouchon->prT,
		vl_bouchon->autorouteQ,
		(va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
		vl_bouchon->prQ,
	        pa_ech->ech.echangeur.NumeroEchangeur,
                vl_bouchon->ptCarT);
            pa_ech->ech.contexte = C_CONTEXTE_MODIF;
	    return;  
	  }
	  else {
	    XZST_03EcritureTrace(XZSTC_FONCTION, "creation only => ne rien faire");
	    pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	    return;
	  }
	}
	else if (bouchon_bretelle_continuant(pa_ech, va_mode)==XDC_OK) {
	    vl_bouchon = pa_ech->ech.bouchon;
	    XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 4 : reveil evolution bouchon %d/%d",
	    		vl_bouchon->numeroQ,
			vl_bouchon->cleQ);
	    sprintf(vl_texte, "Evolution bouchon bretelle detectee");
	    genererReveil(vl_bouchon->numeroQ, vl_bouchon->cleQ, vl_texte,EGAB_TRACE_TYPE_AJUSTEMENT,
					pa_ech->ech.echangeur.NumeroEchangeur,
					pa_ech->ech.cab->config.NumeroCAP);
	    pa_ech->ech.contexte = C_CONTEXTE_REVEIL;
	    return;
        }
	else if (bouchon_section_courante(pa_ech, va_mode)==XDC_OK) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 5 : creation bouchon T/Q en bretelle");
	  pa_ech->ech.contexte = C_CONTEXTE_CREATION;
	  valide_creation(pa_ech, va_mode, (va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
	  				(va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD);
	  return;
	}
    	break;

    case (C_BOUCHON_BOUT_BRETELLE) :
	XZST_03EcritureTrace(XZSTC_FONCTION, "cas 3 : Bouchon en bout de bretelle debordant en section courante");
	if (pg_liste_bouchons==NULL) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 1 : pas de bouchon en cours => creation FMC T bretelle, Q SC");
	  pa_ech->ech.contexte = C_CONTEXTE_CREATION;
	  valide_creation(pa_ech, va_mode ,(va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
	  		va_mode );
	  return;
	}
	if (bouchon_bretelle(pa_ech, va_mode)==XDC_OK) {
	  vl_bouchon = pa_ech->ech.bouchon;
	  if (va_creation_only != C_CREATION_ONLY) {
	    XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 2 : modif bouchon Q en SC",
	  		vl_bouchon->numeroQ,
			vl_bouchon->cleQ);
	    valide_deplace(pa_ech, va_mode, *vl_bouchon,
		vl_bouchon->autorouteT,
		vl_bouchon->sensT,
		vl_bouchon->prT,
		vl_bouchon->autorouteQ ,
		va_mode,
		vl_bouchon->prQ,
		0,
		vl_bouchon->prT);
            return;
	  }
	  else {
	    XZST_03EcritureTrace(XZSTC_FONCTION, "creation only => ne rien faire");
	    pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	    return;
	  }
	}
	if (bouchon_bretelle_debordant(pa_ech, va_mode)==XDC_OK) {
	 XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 3 : bouchon bretelle debordant SC => ne rien faire; purger reveil");
	  vl_bouchon = pa_ech->ech.bouchon;
	 purgerReveil(vl_bouchon->numeroQ, vl_bouchon->cleQ);
	 pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	 return;
	}
	if (bouchon_bretelle_continuant(pa_ech, va_mode)==XDC_OK) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 4 : bouchon bretelle continuant => ne rien faire");
	  pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	  return;
	}
	if (bouchon_section_courante(pa_ech, va_mode)==XDC_OK) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 5 : bouchon SC => ne rien faire");
	  pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	  return;
	}
	break;

    case (C_BOUCHON_BRETELLE_SC):
	XZST_03EcritureTrace(XZSTC_FONCTION,"cas 4 : bouchon bretelle debordant SC");
	if (pg_liste_bouchons==NULL) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 1 : pas de bouchon en cours => creation FMC T bretelle, Q SC");
	  pa_ech->ech.contexte = C_CONTEXTE_CREATION;
	  valide_creation(pa_ech, va_mode, (va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
	  		va_mode );
	  return;
	}
	if (bouchon_bretelle(pa_ech, va_mode)==XDC_OK) {
	  vl_bouchon = pa_ech->ech.bouchon;
	  if (va_creation_only!= C_CREATION_ONLY) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 2 : modif bouchon Q en SC %d/%d - mode %d",
	  		vl_bouchon->numeroQ,
			vl_bouchon->cleQ,
			va_mode);
	  valide_deplace(pa_ech, va_mode, *vl_bouchon,
		vl_bouchon->autorouteT,
		vl_bouchon->sensT,
		vl_bouchon->prT,
		vl_bouchon->autorouteQ,
		va_mode,
		vl_bouchon->prQ,
		0,
		vl_bouchon->prT);
	  return;
	  }
	  else {
	    XZST_03EcritureTrace(XZSTC_FONCTION, "creation only => ne rien faire");
	    pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	    return;
	  }
	}
	if (bouchon_bretelle_debordant(pa_ech, va_mode)==XDC_OK) {
	 XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 3 : bouchon bretelle debordant SC => ne rien faire");
	 pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	 return;
	}
	if (bouchon_bretelle_continuant(pa_ech, va_mode)==XDC_OK) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 4 : bouchon bretelle continuant => ne rien faire; purger reveil");
	  vl_bouchon = pa_ech->ech.bouchon;
	  purgerReveil (vl_bouchon->numeroQ, vl_bouchon->cleQ);
	  pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	  return;
	}
	if (bouchon_section_courante(pa_ech, va_mode)==XDC_OK) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "sous cas 5 : bouchon SC => creation FMC T bretelle/Q SC");
	  pa_ech->ech.contexte = C_CONTEXTE_CREATION;
	  valide_creation(pa_ech, va_mode ,(va_mode == XDC_SENS_SUD) ? XDC_SENS_SORTIE_SUD : XDC_SENS_SORTIE_NORD,
	  		va_mode );
	  pa_ech->ech.contexte = C_CONTEXTE_RIEN;
	  return;
	}
        break;
 } 
}


int bouchon_bretelle(T_Liste_ECH *pa_ech, int va_mode) {
  T_LISTE_CHAINEE *pl_liste;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : bouchon_bretelle");

  pl_liste = pg_liste_bouchons;

  while (pl_liste != NULL) {
    if ( (pl_liste->evt.ptCarQ == pa_ech->ech.echangeur.NumeroEchangeur) &&
       (pl_liste->evt.ptCarT == pa_ech->ech.echangeur.NumeroEchangeur)  &&
       (pl_liste->evt.sensQ % 2 == va_mode % 2)) {
      XZST_03EcritureTrace(XZSTC_FONCTION, "bouchon bretelle trouve queue %d/%d",
      		pl_liste->evt.numeroQ,
		pl_liste->evt.cleQ);
      pa_ech->ech.bouchon = &(pl_liste->evt);
      pa_ech->ech.contexte = C_CONTEXTE_MODIF;
      return (XDC_OK);
    }
    pl_liste = pl_liste->suivant;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : bouchon_bretelle");
  return (XDC_NOK);
}

int bouchon_section_courante(T_Liste_ECH *pa_ech, int va_mode) {
T_LISTE_CHAINEE *pl_liste;
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : bouchon_section_courante");
  pl_liste = pg_liste_bouchons;
  while (pl_liste != NULL) {
    if ( (pl_liste->evt.ptCarQ ==XDC_POINT_CARACT_AUCUN) &&
         (pl_liste->evt.ptCarT == XDC_POINT_CARACT_AUCUN) &&
	        (pl_liste->evt.sensQ % 2 == va_mode % 2)) {
      XZST_03EcritureTrace(XZSTC_FONCTION, "bouchon SC trouve queue %d/%d",
      		pl_liste->evt.numeroQ,
		pl_liste->evt.cleQ);
      pa_ech->ech.contexte = C_CONTEXTE_MODIF;
      pa_ech->ech.bouchon = &(pl_liste->evt);
      return (XDC_OK);
   }
   pl_liste = pl_liste->suivant;
  }
  return (XDC_NOK);
}

int bouchon_bretelle_debordant(T_Liste_ECH *pa_ech, int va_mode) {
  T_LISTE_CHAINEE *pl_liste;
  XZAOT301_Echangeur vl_ech;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : bouchon_bretelle_debordant");
  vl_ech = pa_ech->ech.echangeur;
  pl_liste = pg_liste_bouchons;
  while (pl_liste != NULL) {
    if ( (pl_liste->evt.ptCarT==pa_ech->ech.echangeur.NumeroEchangeur) &&
         (pl_liste->evt.ptCarQ ==0) &&
		(vl_ech.PR == pl_liste->evt.prQ) &&
	        (pl_liste->evt.sensQ % 2 == va_mode %2)) {
	XZST_03EcritureTrace(XZSTC_FONCTION, "bouchon_bretelle_debordant trouve queue %d/%d",
		pl_liste->evt.numeroQ,
		pl_liste->evt.cleQ);
      pa_ech->ech.contexte = C_CONTEXTE_MODIF;
      pa_ech->ech.bouchon = &(pl_liste->evt);
       return (XDC_OK);
    }
    pl_liste = pl_liste->suivant;
  }
  return (XDC_NOK);
}


int bouchon_bretelle_continuant(T_Liste_ECH *pa_ech, int va_mode) {
  T_LISTE_CHAINEE *pl_liste;
  XZAOT301_Echangeur vl_ech;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : bouchon_bretelle_debordant");
  vl_ech = pa_ech->ech.echangeur;
  pl_liste = pg_liste_bouchons;
  while (pl_liste != NULL) {
    if ( (pl_liste->evt.ptCarT==pa_ech->ech.echangeur.NumeroEchangeur) &&
         (pl_liste->evt.ptCarQ ==XDC_POINT_CARACT_AUCUN) &&
	 (pl_liste->evt.prQ != vl_ech.PR) &&
	        (pl_liste->evt.sensQ % 2 == va_mode %2)) {
	XZST_03EcritureTrace(XZSTC_FONCTION, "bouchon_bretelle_continuant trouve queue %d/%d",
		pl_liste->evt.numeroQ,
		pl_liste->evt.cleQ);
      pa_ech->ech.contexte = C_CONTEXTE_MODIF;
      pa_ech->ech.bouchon = &(pl_liste->evt);
       return (XDC_OK);
    }
    pl_liste = pl_liste->suivant;
  }
  return (XDC_NOK);
}
