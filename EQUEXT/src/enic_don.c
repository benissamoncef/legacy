/*E*/
/*Fichier : $Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/22 15:04:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de gestion des donnees de la tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Mismer.D	version 1.2	09 Fev 1995	:
* Fontaine.C	version 1.3	22 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include "enic_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $ : enic_don" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

EX_FONCT_INSERER   (  ENIC_DONNEES_Tube, et_ins_liste_Tube, pg_debutTube, Numero, Suivant, Suivant )
EX_FONCT_INSERER   (  ENIC_DONNEES_Echangeur, et_ins_liste_Ech, pg_debutEchangeur, NumeroDemiEch, Suivant, Suivant )


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Insertion structure de donnee Capteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_ins_liste_Capteur ( ENIC_DONNEES_Capteur * 	pa_Capt,
			       XDY_Eqt		 	va_NumTube )

/*
* ARGUMENTS EN ENTREE :
*
*  ENIC_DONNEES_Capteur * 	pa_Capt		: Structure capteur a inserer
*  XDY_Eqt		 	va_NumTube	: Numero du tube auquel appartient le capteur
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Recherche du tube auquel appartient le capteur
*   Recherche de la position d'insertion
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $ :  et_ins_liste_Capteur " ;
   ENIC_DONNEES_Tube *		pl_DonCrtTube;
   ENIC_DONNEES_Capteur	*	pl_ExEnr;
   ENIC_DONNEES_Capteur	*	pl_Capt;
   ENIC_DONNEES_Capteur	*	pl_Enr;	

      /* Recherche du tube auquel appartient le capteur */
      for ( pl_DonCrtTube = pg_debutTube; pl_DonCrtTube != NULL; pl_DonCrtTube = pl_DonCrtTube->Suivant)
      {
         /* Si Numero de tube existe */
         if ( pl_DonCrtTube->Numero == va_NumTube )
         {
            /*A si l'alocaltion memoire est ok */
            if ( ( pl_Capt = (ENIC_DONNEES_Capteur *)malloc ( sizeof(ENIC_DONNEES_Capteur) ) ) != NULL)
            {
               /*A copie enegistrement */
               memcpy ( pl_Capt, pa_Capt, sizeof(*pl_Capt) );
    
               /*A pour les enregistrements deja existant */
               for ( pl_Enr = pl_ExEnr = pl_DonCrtTube->ListeCapteur; 
                     pl_Enr != NULL; 
                     pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant )
               {
                  /*A Si le numero du capteur est inferieure */
                  if ( pl_Capt->NumeroCapteur < pl_Enr->NumeroCapteur )
   
                  /*A la place d'insertion est localisee : on stoppe la recherche */
                  break;
               }    

               /*A si la place d'insertion est la tete de liste */
               if ( pl_Enr == pl_DonCrtTube->ListeCapteur )
               {
                  /*A cas particulier de la tete de liste */
                  pl_Capt->Suivant            = pl_DonCrtTube->ListeCapteur;
                  pl_DonCrtTube->ListeCapteur = pl_Capt;
               }
               /*A sinon insertion nominal */
               else
               {
                  pl_Capt->Suivant  = pl_ExEnr->Suivant;
                  pl_ExEnr->Suivant = pl_Capt;
               }
            }
         }
      }

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Insertion structure de donnee Sequence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_ins_liste_Sequence ( ENIC_DONNEES_Sequence *	pa_Sequence,
			        XDY_Eqt			va_NumMaitre,
			        XDY_Mot			va_TypeMaitre )

/*
* ARGUMENTS EN ENTREE :
*
*  ENIC_DONNEES_Sequence * 	pa_Sequence	: Structure sequence a inserer
*  XDY_Eqt			va_NumMaitre	: Numero du tube ou echangeur auquel appartient le Sequence
*  XDY_Mot			va_TypeMaitre	: Type echangeur ou tube
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Recherche du tube ou echangeur auquel appartient le Sequence
*   Recherche de la position d'insertion
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $ : et_ins_liste_Sequence " ;
   ENIC_DONNEES_Tube *		pl_DonCrtTube;
   ENIC_DONNEES_Echangeur *	pl_DonCrtEch;
   ENIC_DONNEES_Sequence *	pl_ExEnr;
   ENIC_DONNEES_Sequence *	pl_Sequence;
   ENIC_DONNEES_Sequence *	pl_Enr;	
   ENIC_DONNEES_Sequence *	pl_Debut  = NULL;
   int				vl_Trouve = XDC_NOK;	


	if ( va_TypeMaitre == XDC_EQT_TUBE )
   	{
      	   /* Recherche du tube auquel appartient la sequence */
      	   for ( pl_DonCrtTube = pg_debutTube; pl_DonCrtTube != NULL; pl_DonCrtTube = pl_DonCrtTube->Suivant)
      	   {
              /* Si Numero de tube existe */
              if ( pl_DonCrtTube->Numero == va_NumMaitre )
              {
                 /* Mise a jour du pointeur de debut de liste sequence */
                 pl_Debut  = pl_DonCrtTube->ListeSequence;
                 vl_Trouve = XDC_OK;
                 break;
              }
           }
        }
        else
        {
           /* Recherche de l'echangeur auquel appartient le FAV */
           for ( pl_DonCrtEch = pg_debutEchangeur; pl_DonCrtEch != NULL; pl_DonCrtEch = pl_DonCrtEch->Suivant)
           {
              /* Si Numero de tube existe */
              if ( pl_DonCrtEch->NumeroDemiEch == va_NumMaitre )
              {
                 /* Mise a jour du pointeur de debut de liste sequence */
                 pl_Debut = pl_DonCrtEch->ListeSequence;
                 vl_Trouve = XDC_OK;
                 break;
              }
           }
	}
   
   	if ( vl_Trouve == XDC_OK )
   	{
            /*A si l'alocaltion memoire est ok */
            if ( ( pl_Sequence = (ENIC_DONNEES_Sequence *)malloc ( sizeof(ENIC_DONNEES_Sequence) ) ) != NULL)
            {
               /*A copie enegistrement */
               memcpy ( pl_Sequence,  pa_Sequence, sizeof(*pl_Sequence) );
    
               /*A pour les enregistrements deja existant */
               for ( pl_Enr = pl_ExEnr = pl_Debut ; 
                     pl_Enr != NULL; 
                     pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant )
               {
                  /*A Si le numero du Sequence est inferieur */
                  if ( pl_Sequence->NumSequence < pl_Enr->NumSequence )
   
                  /*A la place d'insertion est localisee : on stoppe la recherche */
                  break;
               }    

               /*A si la place d'insertion est la tete de liste */
               if ( pl_Enr == pl_Debut  )
               {
                  /*A cas particulier de la tete de liste */
                  pl_Sequence->Suivant = pl_Debut ;
                  if ( va_TypeMaitre == XDC_EQT_TUBE ) 
                     pl_DonCrtTube->ListeSequence = pl_Sequence;
                  else
                     pl_DonCrtEch->ListeSequence  = pl_Sequence;
               }
               /*A sinon insertion nominal */
               else
               {
                  pl_Sequence->Suivant = pl_ExEnr->Suivant;
                  pl_ExEnr->Suivant   = pl_Sequence;
               }
            }
         }

}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Insertion structure de donnee Panneau
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_ins_liste_Panneau ( ENIC_DONNEES_Panneau *	pa_Panneau,
			       XDY_Eqt			va_NumMaitre,
			       XDY_Mot			va_TypeMaitre )

/*
* ARGUMENTS EN ENTREE :
*
*  ENIC_DONNEES_Panneau * 	pa_Panneau	: Structure panneau a inserer
*  XDY_Eqt			va_NumMaitre	: Numero du tube ou echangeur auquel appartient le Panneau
*  XDY_Mot			va_TypeMaitre	: Type echangeur ou tube
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Recherche du tube ou echangeur auquel appartient le Panneau
*   Recherche de la position d'insertion
*   Allocation memoire et insertion dans la liste
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $ : et_ins_liste_Panneau " ;
   ENIC_DONNEES_Tube *		pl_DonCrtTube;
   ENIC_DONNEES_Echangeur *	pl_DonCrtEch;
   ENIC_DONNEES_Panneau	*	pl_ExEnr;
   ENIC_DONNEES_Panneau	*	pl_Panneau;
   ENIC_DONNEES_Panneau	*	pl_Enr;	
   ENIC_DONNEES_Panneau	*	pl_Debut  = NULL;
   int				vl_Trouve = XDC_NOK;	


	switch ( va_TypeMaitre )
   	{
   	   case XDC_EQT_TUBE :
      	      /* Recherche du tube auquel appartient le FAV */
      	      for ( pl_DonCrtTube = pg_debutTube; pl_DonCrtTube != NULL; pl_DonCrtTube = pl_DonCrtTube->Suivant)
      	      {
                 /* Si Numero de tube existe */
                 if ( pl_DonCrtTube->Numero == va_NumMaitre )
                 {
                    /* Mise a jour du pointeur de debut de liste panneau */
                    pl_Debut  = pl_DonCrtTube->ListePanneau;
	 	    pl_DonCrtTube->NbrPanFAV++;
                    vl_Trouve = XDC_OK;
                    break;
                 }
              }
              break;
           
   	      case XDC_EQT_ECH :
              /* Recherche de l'echangeur auquel appartient le FAV */
              for ( pl_DonCrtEch = pg_debutEchangeur; pl_DonCrtEch != NULL; pl_DonCrtEch = pl_DonCrtEch->Suivant)
              {
                 /* Si Numero de tube existe */
                 if ( pl_DonCrtEch->NumeroDemiEch == va_NumMaitre )
                 {
                    /* Mise a jour du pointeur de debut de liste panneau */
                    pl_Debut = pl_DonCrtEch->ListePanneau;
	 	    pl_DonCrtEch->NbrPanFAV++;
                    vl_Trouve = XDC_OK;
                    break;
                 }
              }
              break;
        }
   
   	if ( vl_Trouve == XDC_OK )
   	{
            /*A si l'alocaltion memoire est ok */
            if ( ( pl_Panneau = (ENIC_DONNEES_Panneau *)malloc ( sizeof(ENIC_DONNEES_Panneau) ) ) != NULL)
            {
               /*A copie enegistrement */
               memcpy ( pl_Panneau,  pa_Panneau, sizeof(*pl_Panneau) );
    
               /*A pour les enregistrements deja existant */
               for ( pl_Enr = pl_ExEnr = pl_Debut ; 
                     pl_Enr != NULL; 
                     pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant )
               {
                  /*A Si le numero du Panneau est inferieur */
                  if ( pl_Panneau->NumeroPanneau < pl_Enr->NumeroPanneau )
   
                  /*A la place d'insertion est localisee : on stoppe la recherche */
                  break;
               }    

               /*A si la place d'insertion est la tete de liste */
               if ( pl_Enr == pl_Debut  )
               {
                  /*A cas particulier de la tete de liste */
                  pl_Panneau->Suivant = pl_Debut ;
                  if ( va_TypeMaitre == XDC_EQT_TUBE ) 
                     pl_DonCrtTube->ListePanneau = pl_Panneau;
                  else
                     pl_DonCrtEch->ListePanneau  = pl_Panneau;
               }
               /*A sinon insertion nominal */
               else
               {
                  pl_Panneau->Suivant = pl_ExEnr->Suivant;
                  pl_ExEnr->Suivant   = pl_Panneau;
               }
            }
         }

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Suppression structure de donnee Capteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_sup_liste_Capteur ( ENIC_DONNEES_Capteur *	pa_DebCapteur )

/*
* ARGUMENTS EN ENTREE :
*
*   ENIC_DONNEES_Capteur **	pa_DebCapteur : Debut liste de Capteur a supprimer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation de la memoire pour toutes les structures de la liste
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $ : et_sup_liste_Capteur " ;
   ENIC_DONNEES_Capteur	*	pl_ExCapt;
   ENIC_DONNEES_Capteur	*	pl_Capt;
 
 
   /*A pour chaque enregistrement de la liste */
   for ( pl_ExCapt = pl_Capt = pa_DebCapteur;
         pl_Capt != NULL; 
         pl_ExCapt = pl_Capt )
   {
      /* Liberation memoire */
      pl_Capt = pl_Capt->Suivant;
      free ( pl_ExCapt );
   }
   
   pa_DebCapteur = NULL;

}
   

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Suppression structure de donnee Sequence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_sup_liste_Sequence ( ENIC_DONNEES_Sequence *	pa_DebSequence )

/*
* ARGUMENTS EN ENTREE :
*
*   ENIC_DONNEES_Sequence *	pa_DebSequence : Debut liste de Sequence a supprimer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation de la memoire pour toutes les structures de la liste
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $ : et_sup_liste_Sequence " ;
   ENIC_DONNEES_Sequence *	pl_ExSequence;
   ENIC_DONNEES_Sequence *	pl_Sequence;
 
 
   /*A pour chaque enregistrement de la liste */
   for ( pl_ExSequence = pl_Sequence = pa_DebSequence;
         pl_Sequence != NULL; 
         pl_ExSequence = pl_Sequence )
   {
      /* Liberation memoire */
      pl_Sequence = pl_Sequence->Suivant;
      free ( pl_ExSequence );
   }
   
   pa_DebSequence = NULL;

}
   


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Suppression structure de donnee Panneau
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_sup_liste_Panneau ( ENIC_DONNEES_Panneau *	pa_DebPanneau )

/*
* ARGUMENTS EN ENTREE :
*
*   ENIC_DONNEES_Panneau *	pa_DebPanneau : Debut liste de Panneau a supprimer
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation de la memoire pour toutes les structures de la liste
*
------------------------------------------------------*/
{
   static char *version = "$Id: enic_don.c,v 1.3 1995/05/22 15:04:41 cornu Exp $ : et_sup_liste_Panneau " ;
   ENIC_DONNEES_Panneau	*	pl_ExPanneau;
   ENIC_DONNEES_Panneau	*	pl_Panneau;
 
 
   /*A pour chaque enregistrement de la liste */
   for ( pl_ExPanneau = pl_Panneau = pa_DebPanneau;
         pl_Panneau != NULL; 
         pl_ExPanneau = pl_Panneau )
   {
      /* Liberation memoire */
      pl_Panneau = pl_Panneau->Suivant;
      free ( pl_ExPanneau );
   }
   
   pa_DebPanneau = NULL;

}
