/*E*/
/* Fichier : $Id: xzao109.c,v 1.7 2018/10/26 13:48:19 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/10/26 13:48:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao109.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	08 Sep 1994	: Creation
* volcic	28 Oct 1994	: Modification de recuperation des parametres de la structure  1.2
* ???	03/07/2007 : Modification constante de longueur du code camera  1.3
* JPL	12/1O/2010 : Suppression lecture des PR debut et fin  (DEM 948)  1.4
* JPL	15/1O/2010 : ajout lecture de l'adresse IP  (DEM 954)  1.5
* JPL	14/12/2010 : ajout lecture des champs Protocole, Port et Encoder  (DEM 954)  1.6
* JPL	26/10/18 : Lecture de l'indicateur 'Supprime' en configuration (DEM 1305)  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao109.c,v 1.7 2018/10/26 13:48:19 devgfi Exp $ $Revision: 1.7 $ $Date: 2018/10/26 13:48:19 $ : xzao109" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des cameras et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao109_Lire_Config_Camera ( 	FILE  		*pa_Fd,
				        XDY_FonctionInt pa_FonctionUtil,
				        XDY_Entier   	*pa_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* FILE			pa_Fd
* XDY_FonctionInt	pa_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*pa_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*       XZAOT_ConfCamera 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao109.c,v 1.7 2018/10/26 13:48:19 devgfi Exp $ $Revision: 1.7 $ $Date: 2018/10/26 13:48:19 $ : xzao109_Lire_Config_Camera" ;
   
   XZAOT_ConfCamera	vl_Camera;
   char			vl_Lig[XDC_MAX_CHAR_LEN];
   char			*vl_Texte;
   char			vl_Texte_out[XDC_MAX_CHAR_LEN];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao109: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È pa_Fd
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier pa_Fd 
       */
    
       vl_num_ligne++; 
       
       if (vl_Resultat_out != XZAO_EOF )
       { 
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */
          
          *pa_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */
          
          vl_retcode = (int) strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != XDC_OK) 

          {
             /*A
             ** Extraction des differents champs de la ligne
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Nom camera  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Camera.Nom, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_CodeCam, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Code camera  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Camera.Code, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : LT video  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Camera.NomLTVideo, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Autoroute  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Camera.Autoroute, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : PR  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Camera.PR = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Sens  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
            vl_Camera.Sens = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Cycle  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Camera.Cycle = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Mobile  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Camera.Mobile = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Positionnable  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Camera.Positionnable = atoi ( vl_Texte_out );



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_AdresseIP, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Adresse IP  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             strcpy ( vl_Camera.AdresseIP, vl_Texte_out );
             if ( strcmp ( vl_Texte_out, "X" ) == 0 )
             {
                strcpy ( vl_Camera.AdresseIP, "" );
             }



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Protocole, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Protocole  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             strcpy ( vl_Camera.Protocole, vl_Texte_out );
             if ( strcmp ( vl_Texte_out, "X" ) == 0 )
             {
                strcpy ( vl_Camera.Protocole, "" );
             }



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Port  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Camera.Port = atoi ( vl_Texte_out );



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Encoder, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Encoder  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             strcpy ( vl_Camera.Encoder, vl_Texte_out );
             if ( strcmp ( vl_Texte_out, "X" ) == 0 )
             {
                strcpy ( vl_Camera.Encoder, "" );
             }



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao109 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Camera.Supprime = atoi ( vl_Texte_out );



             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_Camera, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao109: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier pa_Fd 
       ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
       */
       
       {  
          *pa_Resultat = NULL;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd 
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = NULL;

return XDC_OK;

}   

/* Fin de la fonction xzao109_Lire_Config_Camera_Station */

