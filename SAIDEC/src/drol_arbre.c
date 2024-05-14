/*E*/
/*Fichier : $Id: drol_arbre.c,v 1.2 2015/09/01 08:44:00 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date :  $Date: 2015/09/01 08:44:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SAIDEC
------------------------------------------------------
* MODULE DPAC_ARBRE * FICHIER drol_arbre.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	cf drol_arbre.h
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   18/06/15 : ajout h debut et h debut prevu datetime DEM1130 1.2
* JMG	01/09/15 : blocs RVA DEM 1130 1.3
------------------------------------------------------*/

/* fichiers inclus */
#include "drol_arbre.h"

/* definitions de constantes */
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char		*version = "$Id: drol_arbre.c,v 1.2 2015/09/01 08:44:00 pc2dpdy Exp $ $Revision: 1.2 $ $Date: 2015/09/01 08:44:00 $ : drol_arbre" ;
static XDY_Evt		vg_NumEvtOrigine;
static XDY_Horodate	vg_Horodate;
static XDY_Evt		vg_NumEvtCause;
static XDY_Octet	vg_TypeEvtCause;
/*static T_NOEUD		*vg_racine;*/

/* definition de fonctions externes */
void tribulle (int,  int , int , int);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  donne la queue de bouchon associee a une tete de bouchon
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int EvtQueue(NumEvtIn, TypeEvtIn, NumEvtOut, TypeEvtOut)
XDY_Evt		NumEvtIn;
XDY_Octet	TypeEvtIn;
XDY_Evt		*NumEvtOut;
XDY_Octet	*TypeEvtOut;

/*
* ARGUMENTS EN ENTREE :
*	XDY_Evt		NumEvtIn;
*	XDY_Octet	TypeEvtIn;
*
* ARGUMENTS EN SORTIE :
*	XDY_Evt		*NumEvtOut;
*	XDY_Octet	*TypeEvtOut;
*
* CODE RETOUR : 
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   fonction appelee par FctCause et FctConsequence
*
* FONCTION 
*   si l'evenement in est de type Tete de bouchon,
*   on appelle XZAE23.
*
------------------------------------------------------*/
{
	int		code_retour=XDC_OK;
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"EvtQueue IN : NumEvt = %d, CleEvt = %d, TypeEvt = %d",
		 NumEvtIn.numero, NumEvtIn.cle, TypeEvtIn);

	/*A
	**  on teste le type de l'evenement in
	*/
	if ( TypeEvtIn == XZAEC_FMC_TeteBouchon )
	{
		/*! une procedure stockee plus simple suffirait 
		*   seul le numero d'evt queue nous interesse ici !*/
		code_retour = XZAE97_Recuperer_Numero_Queue(vg_Horodate,
							NumEvtIn,
							NumEvtOut);
		if (code_retour == XDC_OK) {
			*TypeEvtOut = XZAEC_FMC_QueueBouchon;
		}
		else
		{
			NumEvtOut->numero = NumEvtIn.numero;
			NumEvtOut->cle = NumEvtIn.cle;
		}
	}
	else
	{
		NumEvtOut->numero = NumEvtIn.numero;
		NumEvtOut->cle = NumEvtIn.cle;
		*TypeEvtOut = TypeEvtIn;
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "EvtQueue OUT : %d %d",
					NumEvtOut->numero,
					NumEvtOut->cle);
	return(code_retour);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction passee en argument a XZAE69
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void FctCause(NumEvt, TypeEvt)
XDY_Evt		NumEvt;
XDY_Octet	TypeEvt;

/*
* ARGUMENTS EN ENTREE :
*	XDY_Evt		NumEvt;
*	XDY_Octet	TypeEvt;
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAE69
*
* FONCTION 
*   on stocke le resultat dans des variables globales
*
------------------------------------------------------*/
{
	XDY_Evt		NumEvtQueue;
	XDY_Octet	TypeEvtQueue;
	static char *version = "@(#)drol_arbre.c	1.12 : FctCause";
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"IN : FctCause NumEvt = %d, CleEvt = %d, TypeEvt = %d",
		NumEvt.numero, NumEvt.cle, TypeEvt);

	/*A
	**  on appelle EvtQueue
	*/
	if ( EvtQueue(NumEvt, TypeEvt, &NumEvtQueue, &TypeEvtQueue) != XDC_OK )
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur dans EvtQueue");

	/*A
	**  on stocke le resultat dans des variables globales
	*/
	vg_NumEvtCause.numero = NumEvtQueue.numero;
	vg_NumEvtCause.cle = NumEvtQueue.cle;
	vg_TypeEvtCause = TypeEvtQueue;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT FctCause");
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  fonction passee en argument a XZAE68
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void FctConsequence(NumEvt, TypeEvt, NumPere, Profondeur)
XDY_Evt		NumEvt;
XDY_Octet	TypeEvt;
XDY_Evt		NumPere;
XDY_Octet	Profondeur;

/*
* ARGUMENTS EN ENTREE :
*	XDY_Evt		NumEvt;
*	XDY_Octet	TypeEvt;
*	XDY_Evt		NumPere;
*	XDY_Octet	Profondeur;
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAE68
*
* FONCTION 
*   creation d'un noeud pointant sur l'evt dans le RTie
*
------------------------------------------------------*/
{
	XDY_Evt		NumEvtQueue;
	XDY_Octet	TypeEvtQueue;
	XDY_Evt		NumPereQueue;
	XDY_Octet	TypePereQueue;
	T_STRING	NomPere;
	T_NOEUD		*Pere;
	T_NOEUD		*Fils;
	XDY_Octet	vl_type;

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"FctConsequence IN : NumEvt = %d, CleEvt = %d, TypeEvt = %d, Profondeur = %d, NumPere = %d, ClePere = %d",
		NumEvt.numero, NumEvt.cle, TypeEvt, Profondeur, NumPere.numero, NumPere.cle);

	/*A
	**  on appelle EvtQueue
	*/
	vl_type = TypeEvt;

	if ( EvtQueue(NumEvt, TypeEvt, &NumEvtQueue, &vl_type) != XDC_OK ){
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur dans EvtQueue");
		vl_type = TypeEvt;
	}

	/*! Comme on ne connait pas letype de l'evt pere, on force l'appel a XZAE23 */
	if (EvtQueue(NumPere, XZAEC_FMC_TeteBouchon, &NumPereQueue, &TypePereQueue) != XDC_OK)
		XZST_03EcritureTrace(XZSTC_WARNING, "Erreur dans EvtQueue pour le pere");

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"FctConsequence NumEvtQueue %d %d, type %d",
		NumEvtQueue.numero,
		NumEvtQueue.cle,
		vl_type);
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"FctConsequence NumPereQueue %d %d",
		NumPereQueue.numero,
		NumPereQueue.cle);

	/*A
	**  on cree la structure C correspondante, juste apres son pere
	*/
		/*B
		**  on cherche le pere
		*/
		Pere = vg_racine;
		while ( (Pere!= NULL) && ( (Pere->NumEvt.numero != NumPereQueue.numero)
						|| (Pere->NumEvt.cle != NumPereQueue.cle)))
		{
			Pere = Pere->Suivant;
		}
		if (Pere == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "%s Le noeud Pere n'existe pas",version);
		}

		/*B
		**  on met a jour le slot cause de l'evt fils
		*/
		else
		{
		/*B
		**  on met en place le chainage
		*/
			XZST_03EcritureTrace(XZSTC_WARNING, "pere %d %d",
					Pere->NumEvt.numero,
					Pere->NumEvt.cle);
			Fils = Pere->Suivant;
			Pere->Suivant = (T_NOEUD *) malloc(sizeof(T_NOEUD));
			Pere->Suivant->NumEvt.numero = NumEvtQueue.numero;
			Pere->Suivant->NumEvt.cle = NumEvtQueue.cle;
			Pere->Suivant->Type = vl_type;
			Pere->Suivant->Precedent = Pere;
			Pere->Suivant->Suivant = Fils;
			if (Fils != NULL)
				Fils->Precedent = Pere->Suivant;
			XZST_03EcritureTrace(XZSTC_WARNING, "fils %d %d",
				Pere->Suivant->NumEvt.numero,
				Pere->Suivant->NumEvt.cle);
		}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "FctConsequence OUT");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   creation de l'arbre des evts lies.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int arbre_creer(XDY_Entier va_numero, XDY_Octet va_cle, XDY_Octet va_type, XDY_Horodate va_horodate)

/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		NumEvt;
*	T_REAL8		CleEvt;
*
* ARGUMENTS EN SORTIE :
* le nom du noeud racine
*
* CODE RETOUR : 
* aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
* - interrogation de la base pour connaitre la cause premiere
* - interrogation de la base pour connaitre les consequences
* de la cause premiere
* - creation d'un objet dans la classe NOEUD par consequence.
*
------------------------------------------------------*/
{
	XDY_Evt		Evt;
	T_STRING	vg_racineIE;
	int vl_indice;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : arbre_creer : evt %d %d",
				va_numero, va_cle);

	for (vl_indice=0;vl_indice<50;vl_indice++) {
		vg_B1[vl_indice]=NULL;
		vg_B2[vl_indice]=NULL;
		vg_B3[vl_indice]=NULL;
		vg_B4[vl_indice]=NULL;
	}

	vg_NumEvtOrigine.numero = va_numero;
	vg_NumEvtOrigine.cle = va_cle;
	vg_Horodate = va_horodate;

	/*A
	**  on appelle XZAE69 pour connaitre la cause premiere
	*/
	Evt.numero = va_numero;
	Evt.cle = va_cle;
	vg_NumEvtCause.numero = va_numero;
	vg_NumEvtCause.cle = va_cle;
	vg_TypeEvtCause = va_type;

	if (XZAE69_Liste_Causes(Evt, va_horodate, FctCause) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne unknown",version);
		return (XDC_NOK);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"%s Cause premiere : NumEvt = %d, CleEvt = %d",
		version,vg_NumEvtCause.numero, vg_NumEvtCause.cle);

	/**  on cree le noeud correspondant a la cause premiere
	*/
	vg_racine = (T_NOEUD *) malloc(sizeof(T_NOEUD));
	vg_racine->NumEvt.numero = vg_NumEvtCause.numero;
	vg_racine->NumEvt.cle = vg_NumEvtCause.cle;
	vg_racine->Type = vg_TypeEvtCause;
	vg_racine->Precedent = NULL;
	vg_racine->Suivant = NULL;

	/*A
	**  on appelle XZAE68 pour connaitre les consequences de la cause premiere
	*/
	if (XZAE68_ListeConsequences(vg_NumEvtCause,va_horodate, 0, FctConsequence) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne unknown",version);
		return (XDC_NOK);
	}

	/*A
	** on renvoie le nom du noeud racine
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT ");
	return (XDC_OK);
}



extern int arbre_trier()
{
  XDY_Sens vl_sens,vl_sens_tmp, vl_sens_ref;
  int vl_indice;
  T_NOEUD *vl_noeud;
  int vl_prevu;
  int vl_termine;
  int vl_indiceB1,vl_indiceB2,vl_indiceB3,vl_indiceB4;

  vl_indiceB1 = 0;
  vl_indiceB2=0;
  vl_indiceB3=0;
  vl_indiceB4=0;

  /*recup sens evt initia*/
  vl_noeud = vg_racine;
      vl_sens = vl_noeud->Localisation.Sens;

  vl_sens_ref = vl_sens;
  if ( (vl_sens==XDC_SENS_ENTREE_SUD) || ((vl_sens==XDC_SENS_SORTIE_SUD))) vl_sens_ref = XDC_SENS_SUD;
  if ( (vl_sens==XDC_SENS_ENTREE_NORD) || ((vl_sens==XDC_SENS_SORTIE_NORD))) vl_sens_ref = XDC_SENS_NORD;
  /*separation des evts en plusieurs blocs*/
  vl_noeud = vg_racine;

  while (vl_noeud != NULL) {

    XZST_03EcritureTrace(XZSTC_WARNING,"arbre_trier %d/%d",vl_noeud->NumEvt.numero,vl_noeud->NumEvt.cle);
    XZST_03EcritureTrace(XZSTC_WARNING,"arbre_trier prevu %d/ termine %d",vl_noeud->Infos_TFM.Prevu,vl_noeud->Infos_TFM.Termine);
    vl_sens =  vl_noeud->Localisation.Sens;
    if ( (vl_sens==XDC_SENS_ENTREE_SUD) || ((vl_sens==XDC_SENS_SORTIE_SUD))) vl_sens = XDC_SENS_SUD;
    if ( (vl_sens==XDC_SENS_ENTREE_NORD) || ((vl_sens==XDC_SENS_SORTIE_NORD))) vl_sens = XDC_SENS_NORD;
    vl_prevu = vl_noeud->Infos_TFM.Prevu;
    vl_termine = vl_noeud->Infos_TFM.Termine;


    /*if (vl_noeud->Infos_TFM.Termine==1) goto pos_suivant;*/

    if (vl_noeud->Infos_TFM.Prevu==0) {
      if (vl_sens == vl_sens_ref) {
         vg_B1[vl_indiceB1]=vl_noeud;
	 vl_indiceB1++;
	 XZST_03EcritureTrace(XZSTC_WARNING,"arbre_trier B1");
      }
      else {
         vg_B2[vl_indiceB2]=vl_noeud;
	 vl_indiceB2++;
	 XZST_03EcritureTrace(XZSTC_WARNING,"arbre_trier B2");
      }
    }
    else {
      if (vl_sens == vl_sens_ref) {
         vg_B3[vl_indiceB3]=vl_noeud;
	 vl_indiceB3++;
	 XZST_03EcritureTrace(XZSTC_WARNING,"arbre_trier B3");
      }
      else {
         vg_B4[vl_indiceB4]=vl_noeud;
	 vl_indiceB4++;
	 XZST_03EcritureTrace(XZSTC_WARNING,"arbre_trier B4");
      }
    }
pos_suivant:
    vl_noeud = vl_noeud->Suivant;
  }

  /*tribulle de chaque bloc suivant horodate de debut*/
  tribulle(vl_indiceB1, vl_indiceB2, vl_indiceB3, vl_indiceB4);

  return (XDC_OK);
}


void tribulle (int va_max1,int va_max2, int va_max3, int va_max4)
{
  int vl_indice;
  int en_desordre = 1;
  int i,j;
  XDY_Horodate vl_d,vl_d2;

  for (vl_indice=0; vl_indice < va_max1 ; vl_indice ++) {
    XZST_03EcritureTrace(XZSTC_WARNING, "B1 : evt %d / %d",vg_B1[vl_indice]->NumEvt.numero,
						vg_B1[vl_indice]->NumEvt.cle);
  }

  for (i = 0; (i < va_max1) && en_desordre; ++i)
  {
    /*en_desordre = 0;*/
    for (j = 1; j < (va_max1 - i); ++j) {
	if (vg_B1[j-1]->Infos_TFM.HDebut > vg_B1[j]->Infos_TFM.HDebut)
	{
	  T_NOEUD *temp = vg_B1[j-1];
	  vg_B1[j-1] = vg_B1[j];
	  vg_B1[j] = temp;
	  en_desordre = 1;
	}
    }
  }

  for (vl_indice=0; vl_indice < va_max1 ; vl_indice ++) {
    XZST_03EcritureTrace(XZSTC_WARNING, "B1 trie : evt %d / %d",vg_B1[vl_indice]->NumEvt.numero,
						vg_B1[vl_indice]->NumEvt.cle);
  }

  en_desordre = 1;

  for (vl_indice=0; vl_indice < va_max2 ; vl_indice ++) {
    XZST_03EcritureTrace(XZSTC_WARNING, "B2 : evt %d / %d / %lf",vg_B2[vl_indice]->NumEvt.numero,
						vg_B2[vl_indice]->NumEvt.cle,vg_B2[vl_indice]->Infos_TFM.HDebut);
  }

  for (i = 0; (i < va_max2) && en_desordre; ++i)
  {
    /*en_desordre = 0;*/
    for (j = 1; j < (va_max2 - i); ++j) {
	if (vg_B2[j-1]->Infos_TFM.HDebut > vg_B2[j]->Infos_TFM.HDebut)
	{
	  T_NOEUD *temp = vg_B2[j-1];
	  vg_B2[j-1] = vg_B2[j];
	  vg_B2[j] = temp;
	  en_desordre = 1;
	}
    }
  }

  for (vl_indice=0; vl_indice < va_max2 ; vl_indice ++) {
    XZST_03EcritureTrace(XZSTC_WARNING, "B2 trie : evt %d / %d",vg_B2[vl_indice]->NumEvt.numero,
						vg_B2[vl_indice]->NumEvt.cle);
  }


  en_desordre = 1;
  for (vl_indice=0; vl_indice < va_max3 ; vl_indice ++) {
     XZST_03EcritureTrace(XZSTC_FONCTION, "B3 : evt %d / %d",vg_B3[vl_indice]->NumEvt.numero,
					vg_B3[vl_indice]->NumEvt.cle);
  }

  for (i = 0; (i < va_max3) && en_desordre; ++i)
  {
    /*en_desordre = 0;*/
    for (j = 1; j < (va_max3 - i); ++j) {
	if (vg_B3[j-1]->Infos_TFM.HDebut_Prevu > vg_B3[j]->Infos_TFM.HDebut_Prevu)
	if (vl_d > vl_d2)
	{
	  T_NOEUD *temp = vg_B3[j-1];
	  vg_B3[j-1] = vg_B3[j];
	  vg_B3[j] = temp;
	  en_desordre = 1;
	}
    }
  }

  en_desordre=1; 
  for (vl_indice=0; vl_indice < va_max4 ; vl_indice ++) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "B4 : evt %d / %d",vg_B4[vl_indice]->NumEvt.numero,
						vg_B4[vl_indice]->NumEvt.cle);
  }

  for (i = 0; (i < va_max4) && en_desordre; ++i)
  {
    /*en_desordre = 0;*/
    for (j = 1; j < (va_max4 - i); ++j) {
	if (vg_B4[j-1]->Infos_TFM.HDebut > vg_B4[j]->Infos_TFM.HDebut)
	{
	  T_NOEUD *temp = vg_B4[j-1];
	  vg_B4[j-1] = vg_B4[j];
	  vg_B4[j] = temp;
	  en_desordre = 1;
	}
    }
  }

  for (vl_indice=0; vl_indice < va_max4 ; vl_indice ++) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "B4 trie : evt %d / %d",vg_B4[vl_indice]->NumEvt.numero,
						vg_B4[vl_indice]->NumEvt.cle);
  }


}
