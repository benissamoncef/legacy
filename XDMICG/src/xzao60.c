/*E*/
/*Fichier : $Id: xzao60.c,v 1.5 1994/12/15 14:57:04 volcic Exp $      Release : $Revision: 1.5 $        Date : $Date: 1994/12/15 14:57:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao60.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	08 Sep 1994	: Creation
* volcic        01 Nov 1994     : Modification entete (v1.2)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao60.c,v 1.5 1994/12/15 14:57:04 volcic Exp $ : xzao60" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des pentes et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int xzao60_Lire_Config_Pente ( 	FILE  		*va_Fd,
				      	XDY_FonctionInt va_FonctionUtil,
				        XDY_Entier   	*va_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fichier		va_Fd
* XDY_Fonction		va_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*va_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*    Appelle la fonction utilisateur avec les arguments:
*       XZAOT_ConfPente 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao60.c,v 1.5 1994/12/15 14:57:04 volcic Exp $ : xzao60_Lire_Config_Pente" ;
   
   XZAOT_ConfPente	vl_Pente;
   char			vl_Lig[260];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   int			vl_NbCarLu;
   int			vl_Longueur;
   XDY_Entier		vl_Resultat_out;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (va_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao60: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È va_Fd
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( va_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++; 
       
       if (vl_Resultat_out != XZAO_EOF )
       { 
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */
          
          *va_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presenece du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != 0) 

          {
             /*A
             ** Extraction des differents champs de vl_District
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Pente.Creation = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Pente.NomPortion, vl_Texte_out  );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Pente.PRdebut = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Pente.PRfin = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Pente.Pente = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao60 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
              strcpy ( vl_Pente.NomPente, vl_Texte_out );
             
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (va_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *va_FonctionUtil ) ( vl_Pente, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao60: appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao60: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier va_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
          *va_Resultat = NULL;
          XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao60: echec de l'appel de la fonction utilisateur");
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier va_Fd 
   ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
   */
   
*va_Resultat = NULL;

return ( XDC_OK );

}     
/* Fin de la fonction xzao60_Lire_Config_Pente */

