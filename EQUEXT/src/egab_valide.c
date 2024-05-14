/*E*/
/*Fichier :  $Id: egab_valide.c,v 1.3 2020/11/03 18:09:33 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 18:09:33 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_valide.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de valide de la regulation >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/0118	creation GABEB 1.1
* JMG	26/10/18 gestion reveils 1.2
* GR    08/07/2020 EMS 1.3
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include "egab_valide.h"
#include "egab_cfg.h"
#include "egab_util.h"
#include "xzae567.h"
#include "xzaac.h"
#include "xzae560.h"
#include "xzit.h"
#include "xzae571.h"

#define C_FILE 	"/produits/migrazur/appliSD/fichiers/deq/gabeb.txt"

extern XDY_NomSite                      vg_NomSite;
extern XDY_District                     vg_NumeroSite;
XDY_Octet vm_mode;
extern T_Liste_ECH *pg_ech;

void valide_acquittement_cb (T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);

XDY_Entier acquitter_Alerte(XDY_Eqt va_cab, XDY_TexteAlerte pa_texte_alerte,
		XDY_Entier va_type_validation, XDY_Entier);


int valide_init()
{ 
  char        vl_Lig[255];
  FILE *pl_fd;
  int          vl_retcode = 0;
  XDY_Entier vl_Resultat_out = 0;

  /*A  Armement Callback sur Reception acquittement alerte*/
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ACQUITTE_GABEB),
			valide_acquittement_cb,(T_CB_ARG)NULL) == NULL)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_ACQUITTE_GABEB non cree");
    return (XDC_NOK);
  }

  /*lecture fichier pour connaitre le mode de fonctionnement*/
  vm_mode = C_MANUEL;
  pl_fd = fopen(C_FILE,"r");

  if (pl_fd==NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb lecture %s => mode MANUEL force",C_FILE);
    return (XDC_OK);
  }

  vl_retcode = XZAO000_LireLigneFich ( pl_fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out );
  if (sscanf(vl_Lig, "%d",&vm_mode)!=1) {
     XZST_03EcritureTrace(XZSTC_WARNING, "pb lecture %s => mode MANUEL force",vl_Lig);
     return (XDC_OK);
  }
  else {
    XZST_03EcritureTrace(XZSTC_FONCTION,"mode active : %d",vm_mode);
  }
  return (XDC_OK) ;
}

int valide_termine(T_Liste_ECH *pa_ech, int va_mode, T_Bouchon va_bouchon) {
  char vl_texte[100];
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : valide_termine (%d/%d)",
  			va_bouchon.numeroQ, va_bouchon.cleQ);

  if (vm_mode!=C_AUTO) {
    sprintf(vl_texte, "Fin de la FMC Bouchon %d/%d",va_bouchon.numeroQ, va_bouchon.cleQ);
    genererAlerte(pa_ech, va_bouchon, va_mode, C_ALERTE_FIN, vl_texte);
  }
  else {
    XZST_03EcritureTrace(XZSTC_FONCTION, "mode auto => on termine la FMC");
    terminerFMC(va_bouchon, pa_ech);
  }
  return (XDC_OK);
}

int valide_creation(T_Liste_ECH *pa_ech, int va_mode, int va_sensT, int va_sensQ) {
  XDY_Autoroute vl_autoroute; 
  XDY_PR vl_pr;
  XZAOT301_Echangeur vl_ech;
  XDY_Entier vl_numeroQ, vl_numeroT;
  T_Bouchon vl_bouchon;
  char vl_texte[100];

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : valide_creation");

  vl_ech = pa_ech->ech.echangeur;
  vl_autoroute = vl_ech.NumeroAutoroute;
  vl_pr = vl_ech.PR;
  if (vm_mode!=C_AUTO) {
    vl_bouchon.autorouteQ=vl_autoroute;
    vl_bouchon.prQ = vl_pr;
    vl_bouchon.prT = vl_pr;
    vl_bouchon.ptCarQ = vl_ech.NumeroEchangeur;
    vl_bouchon.ptCarT = vl_ech.NumeroEchangeur;
    vl_bouchon.sensQ = va_sensQ;
    vl_bouchon.sensT = va_sensT;
    vl_bouchon.autorouteT = vl_autoroute;
    sprintf(vl_texte,"Creation FMC Bouchon : %s-%d-%d %s-%d-%d",
		egab_NomAuto(vl_bouchon.autorouteQ),
		vl_bouchon.sensQ,
		vl_bouchon.prQ,
		egab_NomAuto(vl_bouchon.autorouteT),
		vl_bouchon.sensT,
		vl_bouchon.prT);
    genererAlerte(pa_ech, vl_bouchon, va_mode, C_ALERTE_CREATION, vl_texte);
  }
  else
    return (creerFMC(vl_autoroute, va_sensQ, vl_pr,
  	vl_autoroute, va_sensT, vl_pr, 
	 vl_ech.NumeroEchangeur,vl_ech.NumeroEchangeur,&vl_numeroQ, &vl_numeroT,pa_ech,0));
} 

int valide_deplace(T_Liste_ECH *pa_ech,int va_mode, T_Bouchon va_bouchon,
		XDY_Autoroute va_autorouteT, XDY_Sens va_sensT, XDY_PR va_prT,
		XDY_Autoroute va_autorouteQ, XDY_Sens va_sensQ, XDY_PR va_prQ,
		XDY_Eqt va_ptcarQ, XDY_Eqt va_ptcarT) {
  char vl_texte[100];
  T_Bouchon vl_bouchon;
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : valide_deplace");

  if (vm_mode==C_MANUEL) {
    vl_bouchon = va_bouchon;
    vl_bouchon.autorouteQ= va_autorouteQ;
    vl_bouchon.autorouteT= va_autorouteT;
    vl_bouchon.prQ = va_prQ;
    vl_bouchon.prT = va_prT;
    vl_bouchon.sensQ = va_sensQ;
    vl_bouchon.sensT = va_sensT;
    vl_bouchon.ptCarQ = va_ptcarQ;
    vl_bouchon.ptCarT = va_ptcarT;
    sprintf(vl_texte,"nouvelle localisation FMC Bouchon  %s-%d-%d %s-%d-%d",
    	egab_NomAuto(vl_bouchon.autorouteQ),
    	vl_bouchon.sensQ,
    	vl_bouchon.prQ,
    	egab_NomAuto(vl_bouchon.autorouteT),
	    vl_bouchon.sensT,
	    vl_bouchon.prT);
    genererAlerte(pa_ech, vl_bouchon, va_mode, C_ALERTE_MODIF, vl_texte);
  }
  else {
    XZST_03EcritureTrace(XZSTC_FONCTION, "mode auto => on deplace la FMC");
    return (deplacerFMC(pa_ech, va_mode, va_bouchon, 
		va_autorouteT, va_sensT, va_prT,
		va_autorouteQ, va_sensQ, va_prQ));
  }
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    valide_acquittement_cb (T_IPC_CONN      va_Cnx,
                T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                T_CB_ARG                        pa_ARG)

/*
 * ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ICREER_ALERTE
*
* FONCTION
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS 
   T_STR vl_texte_in;
   T_INT2 vl_acquitement_in;
   T_INT4 vl_numEqt_in;
   T_INT4  vl_numAlerte;
#else
   XDY_TexteAlerte vl_texte_in;
   XDY_Mot vl_acquitement_in;
   XDY_Entier vl_numEqt_in;
   XDY_Entier vl_numAlerte;
#endif

XDY_Entier vl_acquitement;
XDY_Eqt vl_numEqt;
XDY_TexteAlerte vl_texte;
XZST_03EcritureTrace( XZSTC_FONCTION, "IN : valide_acquittement_cb ");

if(TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numEqt_in,
                                                                T_IPC_FT_INT2,&vl_acquitement_in,
                                                                T_IPC_FT_STR, &vl_texte_in,
                                                                T_IPC_FT_INT4, &vl_numAlerte,
                                                                NULL))
{
                vl_numEqt = (XDY_Eqt)vl_numEqt_in;
                vl_acquitement = (XDY_Entier)vl_acquitement_in;
                strcpy(vl_texte, vl_texte_in);

                XZST_03EcritureTrace(XZSTC_INFO," Message recupere %d %d %s %d",
                                                vl_numEqt,vl_acquitement,vl_texte, vl_numAlerte);
}else
        {
                XZST_03EcritureTrace(XZSTC_WARNING, " valide_acquittement_cb: Impossible de recuperer le message");
                return;
        }

acquitter_Alerte(vl_numEqt, vl_texte, vl_acquitement, vl_numAlerte);
XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_acquittement_cb");
}

XDY_Entier acquitter_Alerte(XDY_Eqt va_cab, XDY_TexteAlerte pa_texte_alerte,
                                                        XDY_Entier va_type_validation,
                                                        XDY_Entier va_num_alerte){
XDY_Booleen va_alerte_validee;
T_Liste_Generique *pl_listeGenerique;
T_Liste_Generique *pl_listeGenerique2;
T_Liste_Generique *pl_a_supprimer;
T_Liste_ECH *pl_ech =NULL;
T_Alerte *pl_alerte;
T_CAB *pl_cab1;
XDY_Entier vl_numeroQ, vl_numeroT;
T_Bouchon vl_bouchon;
XDY_Octet vl_cle;
XDY_Entier vl_numero;
char *vl_str;
int vl_autT, vl_prT, vl_sensT, vl_autQ, vl_prQ, vl_sensQ;
char vl_nom1[10];
char vl_nom2[10];
        XZST_03EcritureTrace( XZSTC_FONCTION, "IN : acquitter_Alerte ");

        va_alerte_validee = (va_type_validation == XDC_ALT_VRAIE || va_type_validation == XDC_ALT_VRAIE_EVT);

	/* On Recherche l'alerte qui a ete acquitee */
	pl_ech = pg_ech;

  	while (pl_ech !=NULL) {
		pl_cab1 = pl_ech->ech.cab;
		if (pl_cab1!=NULL) {
			if (pl_cab1->config.NumeroCAP==va_cab)
				break;
		}
		pl_ech = pl_ech->suivant;
	}
	if (pl_ech==NULL) {
		XZST_03EcritureTrace( XZSTC_FONCTION,"pas d echangeur %d",va_cab);
		return (XDC_NOK);	
	}

	pl_listeGenerique =pl_ech->ech.ListeAlertes;

        while (pl_listeGenerique != NULL){
		if ( !strcmp(((T_Alerte*)pl_listeGenerique->element)->alerte_envoyee, pa_texte_alerte) )
			break;
		Liste_Generique_Suivant(pl_listeGenerique);
	}

        if (pl_listeGenerique == NULL){
                XZST_03EcritureTrace( XZSTC_WARNING, "acquitter_Alerte : L'alerte \"%s\" n'existe pas", pa_texte_alerte);
                return XDC_NOK;
        }

	pl_alerte = (T_Alerte *)pl_listeGenerique->element;
	XZST_03EcritureTrace(XZSTC_FONCTION,"Traitement Acquitement %s pour Alerte Type %d",
		va_alerte_validee==XDC_VRAI?"POSITIF":"NEGATIF",
		pl_alerte->sous_type_alerte_envoyee);

	if ((pl_alerte->sous_type_alerte_envoyee==C_ALERTE_FIN) ||
		(pl_alerte->sous_type_alerte_envoyee==C_ALERTE_MODIF)) {
		vl_numero = pl_alerte->bouchon.numeroQ;
		vl_cle = pl_alerte->bouchon.cleQ;
	}
	else {
		vl_numero=0;
		vl_cle = 0;
	}
trace_process(EGAB_TRACE_TYPE_VALIDATION,
                        pl_ech->ech.echangeur.NumeroEchangeur,
                        vl_numero,vl_cle,
                        pl_cab1->config.NumeroCAP,
                        pl_ech->ech.contexte,
                        va_num_alerte,va_alerte_validee,
                        va_alerte_validee==XDC_VRAI?"Validation alerte":"Refus alerte");

if (va_alerte_validee==XDC_VRAI) {
	vl_bouchon = pl_alerte->bouchon;
	switch (pl_alerte->sous_type_alerte_envoyee) {
		case (C_ALERTE_FIN) : 
			terminerFMC(pl_alerte->bouchon, pl_ech);
			break;

		case (C_ALERTE_CREATION) :
			creerFMC(vl_bouchon.autorouteQ,
				vl_bouchon.sensQ,
				vl_bouchon.prQ,
				vl_bouchon.autorouteT,
				vl_bouchon.sensT,
				vl_bouchon.prT,
				vl_bouchon.ptCarQ,
				vl_bouchon.ptCarT,
				&vl_numeroQ, &vl_numeroT, pl_ech, va_num_alerte);
			break;

		case (C_ALERTE_MODIF) :
			vl_str = strstr (pa_texte_alerte,"Bouchon")+9;
			XZST_03EcritureTrace( XZSTC_WARNING,"STR %s",vl_str);
			if (sscanf(vl_str,"%[^-]-%d-%d %[^-]-%d-%d",
					vl_nom1, &vl_sensQ, &vl_prQ,
					vl_nom2, &vl_sensT, &vl_prT) == 6)
				deplacerFMC(pl_ech,
					0,
					vl_bouchon,
					 egab_NumAuto(vl_nom2),
					vl_sensT,
					vl_prT,
					egab_NumAuto(vl_nom1),
					vl_sensQ,
					vl_prQ);
			break;
	}	
}

XZST_03EcritureTrace(XZSTC_FONCTION, "avant suppression alerte");
/* On Recherche l'alerte qui a ete acquitee */
        pl_listeGenerique = pl_ech->ech.ListeAlertes;
        while (pl_listeGenerique != NULL){
                if ( !strcmp(((T_Alerte *)pl_listeGenerique->element)->alerte_envoyee, pa_texte_alerte) ) {
                        pl_a_supprimer = pl_listeGenerique;

	pl_a_supprimer = pl_listeGenerique;
                        /*suppression en tete*/
                        if (pl_a_supprimer == pl_ech->ech.ListeAlertes) {
                                pl_ech->ech.ListeAlertes = pl_ech->ech.ListeAlertes->suivant;
                                free(pl_a_supprimer);
                                return (XDC_OK);
                        }
                        else {
                                pl_listeGenerique2 = pl_ech->ech.ListeAlertes;
                                while (pl_listeGenerique2->suivant != NULL){
                                        if ( !strcmp(((T_Alerte *)pl_listeGenerique2->suivant->element)->alerte_envoyee, pa_texte_alerte) ) {
                                                pl_listeGenerique2->suivant = pl_listeGenerique2->suivant->suivant;
                                                free(pl_a_supprimer);
                                                return (XDC_OK);
                                        }
                                        Liste_Generique_Suivant(pl_listeGenerique2);
                                }
                        }
                        break;
                }
                Liste_Generique_Suivant(pl_listeGenerique);
        }

XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : acquitter_Alerte ");

	return (XDC_OK);

}
