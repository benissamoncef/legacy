/*E*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit * FICHIER scon_lit.c
******************************************************
* DESCRIPTION DU MODULE :
*
* ce module contient les fonctions de lecture des champs
* de boites de dialogue utilisees par la tache tsconf
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  30 Aou 1994	: Creation
* T.Milleville  09 Avr 1995 	: Modification pour tenir
*	compte des taches "Shell Script" V1.7
* T.Milleville  25 Jul 1995 	: Mise a jour entete V1.8
* T.Milleville  25 Mar 1996 	: Suppression recuperation
*	Priorite, Type profil et Device Restaiuration  V1.9
* P.Niepceron  02 Avr 1996 	: Suppression recuperation
*	Chemin des traces  V1.10
* P.Niepceron  22 Aou 1996 	: Ajout type PC simplifie  V1.11
* P.Niepceron  16 Dec 1997	: Ajout type PC2 +STA V1.12
* P.Niepceron   01 Jul 1998     : Ajout du type de machine Historique (dem/1697) v1.13
*******************************************************/

/* fichiers inclus */

#include "scon_ser.h"
#include "scon_var.h"

#include "scon_lit.h"

/* definitions de constantes */

#define CHEMIN_TRACES XDC_PATH_MIGRAZURHOME "/traces"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : scon_lit" ;

/* declaration de fonctions internes */



/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_nomMachine a partir
*  des donnees saisies dans le champs texte de BDCMA
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
int LitChampsCMA( XZSCT_NomMachine va_nomMachine )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsCMA" ;
   char *text;
    
   text = XmTextFieldGetString( BDCMA_CH_CAL );
	
   strcpy( va_nomMachine, text );
   XtFree( text );
	       
   if ( strcmp( va_nomMachine, "" ) == 0 )
     return ( CG_VIDE );
   else
     return ( CG_NONVIDE );
}

/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_nomMachine a partir
*  des donnees saisies dans le champs texte de BDCOMA
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
int LitChampsCOMA( XZSCT_NomMachine va_nomMachine_dest,
			XZSCT_NomMachine va_nomMachine_exped)
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsCOMA" ;
   char *text;
   
	/*A Lecture de la machine a creer */
   	text = XmTextFieldGetString( BDCOMA_CH_CAL_DEST );
   	strcpy( va_nomMachine_dest, text );
   	XtFree( text );

	/*A Lecture de la machine a copier */
   	text = XmTextFieldGetString( BDCOMA_CH_CAL_EXPED );
   	strcpy( va_nomMachine_exped, text );
   	XtFree( text );

   if ( !strcmp( va_nomMachine_dest, "") 
		&& !strcmp( va_nomMachine_exped, "") )
     return ( CG_VIDE );
   else
     return ( CG_NONVIDE );
}

/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction remplit les champs de pa_strucMac a partir
*  des donnees saisies dans le champs texte de BDMAC
*
******************************************************
* SEQUENCE D'APPEL :
*/

int LitChampsMAC( XZSXT_Machine *pa_strucMac )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/

{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsMAC" ;
   int    i;
   #define  vl_NbChamps 	12
   char   *vl_text[vl_NbChamps];
       
   /* on recupere les champs texte de BDMAC          */

   vl_text[1]  = XmTextFieldGetString( BDMAC_CH_SIT );
   vl_text[2] =  XmTextFieldGetString( BDMAC_CH_IMP );
   vl_text[3]  = XmTextFieldGetString( BDMAC_CH_IMPSEC );
   vl_text[4] =  XmTextFieldGetString( BDMAC_CH_MIM );
   vl_text[5]  = XmTextFieldGetString( BDMAC_CH_MAC );
   vl_text[6]  = XmTextFieldGetString( BDMAC_CH_PSR );
   vl_text[7]  = XmTextFieldGetString( BDMAC_CH_CHE );
   vl_text[8]  = XmTextFieldGetString( BDMAC_CH_TAI );
   vl_text[9]  = XmTextFieldGetString( BDMAC_CH_PER );
   vl_text[10]  = XmTextFieldGetString( BDMAC_CH_DAB );

   /* on stocke ces champs dans la structure pa_strucMac  */
   /* en transformant les chaines en entiers si necessaire*/

   strcpy( pa_strucMac->NomSite, vl_text[1][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[1] );

	/*A Teste si le Nom du Site ne depasse pas 2 caracteres */
	if ( strlen(pa_strucMac->NomSite) > 2)
	{
		/*B Site incorrecte alors on affiche ? */
		strcpy( pa_strucMac->NomSite,CG_CHAINE_DEFAUT);

		/*B On sort sans enregistrement */
		return(CG_SITE);
	}

   strcpy( pa_strucMac->NomImprimante, vl_text[2][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[2] );
   strcpy( pa_strucMac->NomImprimanteSec, vl_text[3][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[3] );
   strcpy( pa_strucMac->NomMachineImp, vl_text[4][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[4] );
/*251196
   strcpy( pa_strucMac->PathTrace, vl_text[7][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[7] );
*/
   strcpy( pa_strucMac->PathTrace, CHEMIN_TRACES );

   pa_strucMac->MaxTailleFicTrace = atoi ( vl_text[8] );
   pa_strucMac->PeriodeTestReseau = atoi ( vl_text[9] );
   pa_strucMac->DureeAbsenceCourte = atoi ( vl_text[10] );
   strcpy( pa_strucMac->NomMachine, vl_text[5][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[5] );
   strcpy( pa_strucMac->NomSite, vl_text[1][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[1] );
   strcpy( pa_strucMac->NomPeriphSR, vl_text[6][0] == '\0' ? 
		CG_CHAINE_DEFAUT : vl_text[6] );

   /* quel toggle button est selectionne      */
   /* parmi les 2 du bloc "prioritaire" ?     */
/*251196
   if ( (XmToggleButtonGetState( BDMAC_TO_NON ) ) == True )
     pa_strucMac->Priorite = XZSCC_ESCLAVE ;
   else
     if ( (XmToggleButtonGetState( BDMAC_TO_OUI ) ) == True )
       pa_strucMac->Priorite = XZSCC_MAITRE ;
*/
/*251196   */
     pa_strucMac->Priorite = XZSCC_ESCLAVE ;


  /* quel toggle button est selectionne          */
  /* parmi les 5 du bloc "type de machine" ?     */
  if ( (XmToggleButtonGetState( BDMAC_TO_ST ) ) == True )
    pa_strucMac->TypeMachine = XZSAC_TYPEM_ST ;
  else
  if ( (XmToggleButtonGetState( BDMAC_TO_STA ) ) == True )
    pa_strucMac->TypeMachine = XZSAC_TYPEM_STA ;
  else
  if ( (XmToggleButtonGetState( BDMAC_TO_HIS ) ) == True )
    pa_strucMac->TypeMachine = XZSAC_TYPEM_HIS ;
  else
    if ( (XmToggleButtonGetState( BDMAC_TO_SD ) ) == True )
      pa_strucMac->TypeMachine = XZSAC_TYPEM_SD ;
    else
      if ( (XmToggleButtonGetState( BDMAC_TO_SC ) ) == True )
	pa_strucMac->TypeMachine = XZSAC_TYPEM_SC ;
    else
      if ( (XmToggleButtonGetState( BDMAC_TO_LT ) ) == True )
	pa_strucMac->TypeMachine = XZSAC_TYPEM_LT ;
    else
      if ( (XmToggleButtonGetState( BDMAC_TO_SP ) ) == True )
	pa_strucMac->TypeMachine = XZSAC_TYPEM_SP ;
    else
      if ( (XmToggleButtonGetState( BDMAC_TO_PCS ) ) == True )
	pa_strucMac->TypeMachine = XZSAC_TYPEM_PCS ;
    else
      if ( (XmToggleButtonGetState( BDMAC_TO_PC2 ) ) == True )
        pa_strucMac->TypeMachine = XZSAC_TYPEM_PC2 ;
    else
       if ( (XmToggleButtonGetState( BDMAC_TO_PC2LT ) ) == True )
	 pa_strucMac->TypeMachine = XZSAC_TYPEM_PC2LT ;

   /* quel toggle button est selectionne      */
   /* parmi les 4 du bloc Type Profil" ?     */
/*251196
   if ( (XmToggleButtonGetState( T_PRO_NUL ) ) == True )
     pa_strucMac->TypeProfil = XZSCC_TYPEP_NUL;
   else
     if ( (XmToggleButtonGetState( T_PRO_ADM ) ) == True )
     	pa_strucMac->TypeProfil = XZSCC_TYPEP_ADM;
   else
     if ( (XmToggleButtonGetState( T_PRO_GIT ) ) == True )
     	pa_strucMac->TypeProfil = XZSCC_TYPEP_GIT;
   else
     if ( (XmToggleButtonGetState( T_PRO_OTR ) ) == True )
     	pa_strucMac->TypeProfil = XZSCC_TYPEP_OTR;
*/   
/*251196   */
     pa_strucMac->TypeProfil = XZSCC_TYPEP_NUL;

   /* on libere la memoire allouee par XmTextFieldGetString*/
   for ( i=0; i<11; i++ )
   {
     XtFree( vl_text[i] );
   }
	     
   if ( strcmp( pa_strucMac->NomMachine, "" ) == 0 )
     return( CG_VIDE );
   else
     return( CG_NONVIDE );
}


/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_nomMac et 
*  va_nomTac a partir des donnees saisies
*  dans les champs texte de BDCTM
*
******************************************************
* SEQUENCE D'APPEL :
*/

int LitChampsCTM( XZSCT_NomMachine va_nomMac,
	      XZSCT_NomTache va_nomTac )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsCTM" ;
  char *text;
    
  text = XmTextFieldGetString( BDCTM_CH_CAL );
  strcpy( va_nomMac, text );

  text = XmTextFieldGetString( BDCTM_CH_TAC );
  strcpy( va_nomTac, text );

  XtFree( text );

  if ( (strcmp( va_nomMac, "" ) == 0) || (strcmp( va_nomTac, "" ) == 0) )
    return( CG_VIDE );

  return( CG_NONVIDE );
}


/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int LitChampsTAC( XZSXT_Tache *pa_strucTac )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsTAC" ;
  int    i;
  char   *vl_text[4];

  /* on recupere les champs texte de BDTAC          */
  vl_text[0]  = XmTextFieldGetString( BDTAC_CH_TAC ); 
  vl_text[1]  = XmTextFieldGetString( BDTAC_CH_LRT );
  vl_text[2]  = XmTextFieldGetString( BDTAC_CH_LRL );
  vl_text[3]  = XmTextFieldGetString( BDTAC_CH_TAI );

  if ( strcmp( vl_text[0], "" ) == 0 )
  {
    for ( i=0; i<4; i++ )
    {
      XtFree( vl_text[i] );
    }
    return( CG_VIDE);
  }

  /* on stocke ces champs dans la structure pa_strucTac  */
  /* en transformant les chaines en entiers si necessaire*/

  strcpy( pa_strucTac->NomTache, vl_text[0][0] == '\0' ? CG_CHAINE_DEFAUT : vl_text[0] );
  /*strcpy( pa_strucTac->ServerNode, vl_text[1][0] == '\0' ? CG_CHAINE_DEFAUT : vl_text[1] );*/
  strcpy( pa_strucTac->ListeServer, vl_text[1][0] == '\0' ? CG_CHAINE_DEFAUT : vl_text[1] );
  strcpy( pa_strucTac->ListeLicense, vl_text[2][0] == '\0' ? CG_CHAINE_DEFAUT : vl_text[2] );
  pa_strucTac->TailleFicTrace = atoi ( vl_text[3] );

  /* on libere la memoire allouee par XmTextFieldGetString*/
  for ( i=0; i<4; i++ )
  {
    XtFree( vl_text[i] );
  }

  return( CG_NONVIDE);
}


/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_tailleJournal a partir
*  des donnees saisies dans le champs texte de BDCJA
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
int LitChampsCJA( long *va_tailleJournal )
  
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsCJA" ;
   char *text;
      
   text = XmTextFieldGetString( BDCJA_CH_TAI );
   if ( strcmp( text, "" ) == 0 )
   {
     XtFree( text );
     return( CG_VIDE);
   }

   *va_tailleJournal = atoi ( text ); 
   XtFree( text );

   return ( CG_NONVIDE );
}


/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_nomSite a partir
*  des donnees saisies dans le champs texte de BDSIT
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
int LitChampsSIT( XZSCT_NomSite va_nomSite )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsSIT" ;
  char *text;
     
  text = XmTextFieldGetString( BDSIT_CH_SIT );
	 
  strcpy( va_nomSite, text );
  XtFree( text );
		
  if ( strcmp( va_nomSite, "" ) == 0 )
    return ( CG_VIDE );
  
  return ( CG_NONVIDE );
}
				 
/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_nomMac et
*  va_nomSsy a partir des donnees saisies
*  dans les champs texte de BDSSY
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
int LitChampsSSY( XZSCT_NomSSysteme va_nomSsy,
	      int  		*va_typeMac )
		
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsSSY" ;
  char *text;
		   
  text = XmTextFieldGetString( BDSSY_CH_SSY );
  strcpy( va_nomSsy, text );

  XtFree( text );

  /* quel toggle button est selectionne          */
  /* parmi les 3 du bloc "type de machine" ?     */
  if ( (XmToggleButtonGetState( BDSSY_TO_ST ) ) == True )
    *va_typeMac = XZSAC_TYPEM_ST ;
  else
  if ( (XmToggleButtonGetState( BDSSY_TO_STA ) ) == True )
    *va_typeMac = XZSAC_TYPEM_STA ;
  else
  if ( (XmToggleButtonGetState( BDSSY_TO_HIS ) ) == True )
    *va_typeMac = XZSAC_TYPEM_HIS;
  else
    if ( (XmToggleButtonGetState( BDSSY_TO_SD ) ) == True )
      *va_typeMac = XZSAC_TYPEM_SD ;
    else
      if ( (XmToggleButtonGetState( BDSSY_TO_SC ) ) == True )
	*va_typeMac = XZSAC_TYPEM_SC ;
    else
      if ( (XmToggleButtonGetState( BDSSY_TO_LT ) ) == True )
	*va_typeMac = XZSAC_TYPEM_LT ;
    else
      if ( (XmToggleButtonGetState( BDSSY_TO_SP ) ) == True )
	*va_typeMac = XZSAC_TYPEM_SP ;
    else
      if ( (XmToggleButtonGetState( BDSSY_TO_PCS ) ) == True )
	*va_typeMac = XZSAC_TYPEM_PCS ;
    else
      if ( (XmToggleButtonGetState( BDSSY_TO_PC2 ) ) == True )
	*va_typeMac = XZSAC_TYPEM_PC2;
    else
      if ( (XmToggleButtonGetState( BDSSY_TO_PC2LT ) ) == True )
	*va_typeMac = XZSAC_TYPEM_PC2LT ;

  if ( strcmp( va_nomSsy, "" ) == 0)
     return( CG_VIDE );
					    
  return( CG_NONVIDE );
}

/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_nomMac a partir
*  des donnees saisies dans le champs texte de BDCAL
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
int LitChampsCAL( XZSCT_NomMachine va_nomMac )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsCAL" ;
   char *text;
    
   text = XmTextFieldGetString( BDCAL_CH_CAL );
       
   strcpy( va_nomMac, text );
   XtFree( text );
	    
   if ( strcmp( va_nomMac, "" ) == 0 )
     return ( CG_VIDE );

   return ( CG_NONVIDE );
}

/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_  a partir
*  des donnees saisies dans le champs texte de BDENT
*
******************************************************
* SEQUENCE D'APPEL :
*/

int LitChampsENT( int              *va_typeEntite, 
	      XZSCT_NomSite    va_nomSite,
	      XZSCT_NomEntite  va_nomEntite)

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsENT" ;
  char *text;
  XZSCT_NomTache   vl_nomTache;
  XZSCT_NomMachine vl_nomMachine;
		    
  strcpy( va_nomSite, "" );
  strcpy( va_nomEntite, "" );
  strcpy( vl_nomTache, "" );
  strcpy( vl_nomMachine, "" );

  /* quel toggle button est selectionne          */
  /* parmi les 4 du bloc "type de l'entite" ?    */

  if ( (XmToggleButtonGetState( BDENT_TO_PER ) ) == True )
  {
    /* type = peripherique  */
    *va_typeEntite = XZSAC_TYPE_PERI ;

    /* recuperation du nom du site */
    text = XmTextFieldGetString( BDENT_CH_SI );
    strcpy( va_nomSite, text );

    /* recuperation du nom de l'entite */
    text = XmTextFieldGetString( BDENT_CH_EN );
    strcpy( va_nomEntite, text );

    XtFree( text );

    if ( (strcmp( va_nomSite, "" ) == 0) 
      || (strcmp( va_nomEntite, "" ) == 0) )
      return ( CG_VIDE );
  }
  else
    if ( (XmToggleButtonGetState( BDENT_TO_INT ) ) == True )
    {
      /* type = interface  */
      *va_typeEntite = XZSAC_TYPE_INTE ;

      /* recuperation du nom de l'entite */
      text = XmTextFieldGetString( BDENT_CH_EN );
      strcpy( va_nomEntite, text );

      XtFree( text );

      if ( strcmp( va_nomEntite, "" ) == 0 )
        return ( CG_VIDE );
    }
      else
        if ( (XmToggleButtonGetState( BDENT_TO_TAC ) ) == True )
	{
          /* type = tache temporaire  */
	  *va_typeEntite = XZSAC_TYPE_TTMP ;
      
	  /* recuperation du nom de la machine */
          text = XmTextFieldGetString( BDENT_CH_CA );
          strcpy( vl_nomMachine, text );
      
	  /* recuperation du nom de la tache */
          text = XmTextFieldGetString( BDENT_CH_TA );
          strcpy( vl_nomTache, text );
   
          XtFree( text );

	  sprintf( va_nomEntite, "%s %s", vl_nomMachine, vl_nomTache );

	  if ( (strcmp( vl_nomMachine, "" ) == 0) 
            || (strcmp( vl_nomTache, "" ) == 0) )
            return ( CG_VIDE );
	}

  return( CG_NONVIDE );
}

/*X*/
/* Fichier : $Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1998/09/22 13:59:12 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction renseigne va_nomMac et
*  va_nomTac a partir des donnees saisies
*  dans les champs texte de BDTAS
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
int LitChampsTAS( 
	XZSCT_NomSSysteme va_nomSsy,
      	int *va_typeMac,
      	XZSCT_NomTache va_nomTac ,
	int *va_temporaire)
		
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_lit.c,v 1.13 1998/09/22 13:59:12 gaborit Exp $ : LitChampsTAS" ;
  char *text;
		     
  text = XmTextFieldGetString( BDTAS_CH_SSY );
  strcpy( va_nomSsy, text );
			  
  text = XmTextFieldGetString( BDTAS_CH_TAC );
  strcpy( va_nomTac, text );

  /* quel toggle button est selectionne          */
  /* parmi les 5 du bloc "type de machine" ?     */
  if ( (XmToggleButtonGetState( BDTAS_TO_STA ) ) == True )
    *va_typeMac = XZSAC_TYPEM_STA ;
  else
  if ( (XmToggleButtonGetState( BDTAS_TO_HIS ) ) == True )
    *va_typeMac = XZSAC_TYPEM_HIS;
  else
  if ( (XmToggleButtonGetState( BDTAS_TO_ST ) ) == True )
    *va_typeMac = XZSAC_TYPEM_ST ;
  else
    if ( (XmToggleButtonGetState( BDTAS_TO_SD ) ) == True )
      *va_typeMac = XZSAC_TYPEM_SD ;
    else
      if ( (XmToggleButtonGetState( BDTAS_TO_SC ) ) == True )
	*va_typeMac = XZSAC_TYPEM_SC ;
    else
      if ( (XmToggleButtonGetState( BDTAS_TO_LT ) ) == True )
	*va_typeMac = XZSAC_TYPEM_LT ;
    else
      if ( (XmToggleButtonGetState( BDTAS_TO_SP ) ) == True )
	*va_typeMac = XZSAC_TYPEM_SP ;
    else
      if ( (XmToggleButtonGetState( BDTAS_TO_PCS ) ) == True )
	*va_typeMac = XZSAC_TYPEM_PCS ;
    else
      if ( (XmToggleButtonGetState( BDTAS_TO_PC2 ) ) == True )
	*va_typeMac = XZSAC_TYPEM_PC2 ;
    else
      if ( (XmToggleButtonGetState( BDTAS_TO_PC2LT ) ) == True )
	*va_typeMac = XZSAC_TYPEM_PC2LT ;

  /* quel toggle button est selectionne          */
  /* parmi le type de tache  */
  if ( (XmToggleButtonGetState( Tache_Temporaire) ) == True )
    *va_temporaire = XZSAC_TYPE_TTMP;
  else
   if ( (XmToggleButtonGetState( Tache_Permanente) ) == True )
    *va_temporaire = XZSAC_TYPE_TPER;
  else
   if ( (XmToggleButtonGetState( Tache_ShellScript) ) == True )
    *va_temporaire = XZSAC_TYPE_TSHE;
  
 XtFree( text );
				  
  if ( (strcmp( va_nomSsy, "" ) == 0) || (strcmp( va_nomTac, "" ) == 0) )
    return( CG_VIDE );
					 
  return( CG_NONVIDE );
}

