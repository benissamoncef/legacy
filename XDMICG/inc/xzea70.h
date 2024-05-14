
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int     XZEA70_Abt_Etat_PRV (           XDY_Booleen     va_Abonnement,
                                        XDY_Datagroup   pa_DGInit,
                                        XDY_Fonction    pa_FonctUtilisateur,
                                        XDY_Nom         pa_NomSite,
                                        int *           pa_Resultat );

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen   va_Abonnement           : Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup pa_DGInit               : Nom du DataGroup d'Init
* XDY_Fonction  pa_FonctUtilisateur     : Nom de la fonction utilisateur
* XDY_Nom       pa_NomSite              : Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int           pa_resultat             : Code retour resultat
*
* CODE RETOUR :
*   XDC_OK              Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des PRV d'un site
*
* FONCTION
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/

