/*E*/
/*Fichier : $Id: xzao52.c,v 1.15 2016/07/13 12:43:15 devgfi Exp $      Release : $Revision: 1.15 $        Date : $Date: 2016/07/13 12:43:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao52.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	18 Oct 1994	: Creation
* volcic	18 Oct 1994	: Modif pour test (v1.2)
* volcic	18 Oct 1994	: Modif sscanf (v1.3)
* volcic	18 Oct 1994	: Modif pour test (v1.4)
* volcic	18 Oct 1994	: Modif pour test (v1.5)
* volcic	18 Oct 1994	: Modif pour test (v1.6)
* volcic	18 Oct 1994	: Modif pour test (v1.7)
* volcic	18 Oct 1994	: Modif pour test (v1.8)
* volcic	18 Oct 1994	: Modif pour test (v1.9)
* volcic	19 Oct 1994	: Ajout verif coherence (v1.10)
* volcic	21 Oct 1994	: Change verif nom district (v1.12)
* volcic	24 Oct 1994	: Ajout numero district (v1.13)
* C.T.		22 Jan 1998	: Ajout type, serveur, machine district (1545) (v1.14)
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.15
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao52.c,v 1.15 2016/07/13 12:43:15 devgfi Exp $ : xzao52";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des districts et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao52_Lire_Config_District( FILE  		 *va_Fd,
				 XDY_FonctionInt va_FonctionUtil,
				 XDY_Entier   	 *va_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fichier		va_Fd
* XDY_Fonction		va_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*va_Resultat : OK=NULL ou ligne de l'erreur du systeme
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
*       XZAOT_ConfDistrict	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK -> arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao52.c,v 1.15 2016/07/13 12:43:15 devgfi Exp $ : xzao52_Lire_Config_District" ;

   XZAOT_ConfDistrict	vl_District;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   char			vl_Distr[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode;
   int  		vl_num;
   int			vl_NbCarLu;
   XDY_Entier		vl_Resultat_out;
 
   /* Test sur le file descriptor passe en parametre d'entree */
 
   if (va_Fd == NULL)
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao52: fichier de configuration base non existant");
      return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È va_Fd 
   */
 
   while ( (vl_retcode = XZAO000_LireLigneFich ( va_Fd, vl_Lig, sizeof ( vl_Lig ), &vl_Resultat_out ) ) != XZAO_EOF )
   {
      
       
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++; 
          
       vl_retcode = strcspn( vl_Lig, XZAO_COMMENT );

       if (vl_Resultat_out != XZAO_EOF )
       {
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */

          *va_Resultat = vl_num_ligne;

          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */

          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != 0)
          {  
             /*B
             ** Extraction des differents champs de vl_District 
             */
             
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie du district non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao52 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_District.Numero = atoi ( vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie du district non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao52 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_District.NomDistrict, vl_Texte_out );
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie du district non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao52 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_District.Abreviation, vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie du district non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao52 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_District.Type, vl_Texte_out );
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomMachine, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie du district non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao52 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_District.Serveur, vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomMachine, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie du district non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao52 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_District.Machine, vl_Texte_out );
             
          	
             /*B
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (va_FonctionUtil != NULL)
             {
                /*A
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( ( vl_retcode = (*va_FonctionUtil) (vl_District, vl_num_ligne) ) != XDC_OK )
                {
                   XZST_03EcritureTrace(XZSTC_WARNING,"appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }
             
             } 
             else
             {
             
             /*B
             ** Retour du status XDC_NOK et du numero de ligne 
             */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao52: echec de l'appel de la fonction utilisateur");
 
                *va_Resultat = vl_num_ligne;
                return(XDC_NOK);
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

/* Fin de la fonction */
