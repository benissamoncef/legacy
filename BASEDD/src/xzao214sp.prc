/*E*/
/*Fichier : $Id: xzao214sp.prc,v 1.4 1995/01/09 13:18:31 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/01/09 13:18:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao214sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao214sp.h" */

/* static char *version = "@(#)xzao214.c	1.1 06/10/94 : xzao214" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des segments d'une portion
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;214
   @va_Basedd_in       	char(3),
   @va_NumeroPortion_in	smallint
   as 

/* 
*
* SP	XZAO214SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_Mot	va_NumeroPortion_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des segments d'une portion
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
* Retourne numero segment, numero autoroute, coef_trans debut, coef_trans fin, 
* sens, nombre de voies, largeur BAU, tunnel
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	06 Oct 1994	: Creation
* volcic	23 Oct 1994	: Modif table temporaire (v1.2)
------------------------------------------------------*/
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Autoroute	tinyint,
   @vl_Segment		smallint,
   @vl_Num		smallint,
   @vl_PRdebut		int,
   @vl_PRfin		int,
   @vl_PRd		int,
   @vl_PRf		int,
   @vl_Coefd		int, 
   @vl_Coeff		int, 
   @vl_Sens		tinyint, 
   @vl_NbVoies		tinyint, 
   @vl_BAU		tinyint, 
   @vl_Tunnel		bit,
   @vl_NomTube		char(25)		
	 
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NumeroPortion_in = null
    
      return XDC_ARG_INV
   
   else   
   
   begin
      
      select @vl_Num = 0
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Creation d'une table temporaire TMP_SEG pour retorner la liste des segments
         */
         
         create table #TMP_SEG
         ( numero	smallint not null,
           num		smallint not null,
           autoroute	tinyint not null,
           coefd	int not null,
           coeff	int not null,
           sens		tinyint,
           nbvoies	tinyint,
           lBAU		tinyint,
           tunnel	bit,
           nom		char(25) )
           
           
         /*A
         ** Recherche de l'autoroute sur lequel se trouve la portion
         */
         
         select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFG..RES_POR
           where numero = @va_NumeroPortion_in
         
         /*A
         ** Declaration d'un curseur sur la table RES_SEG
         */
         
         select * into #TMP_SEGM from CFG..RES_SEG 
           where portion = @va_NumeroPortion_in
           order by sens, PR_debut
         
         declare Segment cursor 
         for select portion, PR_debut, sens, nombre_de_voies, largeur_BAU, tunnel, nom_tunnel from #TMP_SEGM
               where portion = @va_NumeroPortion_in
	         
	 open Segment
	         
	 fetch Segment into @vl_Segment, @vl_PRd, @vl_Sens, @vl_NbVoies, @vl_BAU, @vl_Tunnel, @vl_NomTube
	 
	 /*A
	 ** Tant que le curseur n'est pas vide
	 */
	 
	 while (@@sqlstatus = 0)
	 begin
	   
	    /*A
	    ** Recherche du PR de fin de segment :
	    ** PRfin(segment) = Prdebut(segment suivant)
	    */
	    
	    select @vl_Num = @vl_Num +1
	    
	    select @vl_PRf = min(PR_debut) from CFG..RES_SEG
	      where portion  = @va_NumeroPortion_in
	        and sens     = @vl_Sens
	        and PR_debut > @vl_PRd
	    
	    if @vl_PRf = null
	    
	       select @vl_PRf = PR_fin from CFG..RES_POR
	         where numero = @va_NumeroPortion_in
	         
	    /*A
	    ** Calcul des coefficients de translation d'un segment
	    */
	    
	    /*B
	    ** Le PRdebut de la portion est inferieur ou egal au PRdebut du segment
	    */
	    
	    if @vl_PRdebut <= @vl_PRd
	    begin
	       
	       select @vl_Coefd = (@vl_PRd - @vl_PRdebut) * 10000 / (@vl_PRfin - @vl_PRdebut )
	       
	       select @vl_Coeff = (@vl_PRf - @vl_PRdebut) * 10000 / (@vl_PRfin - @vl_PRdebut )
	       
	    end
	    
	    else
	    
	    /*B
	    ** Le PRdebut de la portion est superieur au PRdebut du segment
	    */
	    
	    begin
	       
	       select @vl_Coefd = (@vl_PRf - @vl_PRfin ) * 10000 / (@vl_PRdebut - @vl_PRfin )
	       
	       select @vl_Coeff = (@vl_PRd - @vl_PRfin ) * 10000 / (@vl_PRdebut - @vl_PRfin )
	       
	    end
	    
	    /*A
	    ** Insertion des infos pour la liste des segments dans la table temporaire TMP_SEG
	    */
	    
	    insert into #TMP_SEG
	    values ( @vl_Num, @vl_Segment, @vl_Autoroute, @vl_Coefd, @vl_Coeff, @vl_Sens, @vl_NbVoies, @vl_BAU, @vl_Tunnel, @vl_NomTube )
	    
	    fetch Segment into @vl_Segment, @vl_PRd, @vl_Sens, @vl_NbVoies, @vl_BAU, @vl_Tunnel, @vl_NomTube
	 
	 end
	 
	 /*A
	 ** Fermeture du curseur sur la table RES_SEG
	 */
	 
	 close Segment
	 
	 /*A
	 ** Renvoi la liste des segments
	 */
	 
	 select * from #TMP_SEG
	    
      end
      
#ifdef CI

      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Creation d'une table temporaire TMP_SEG pour retorner la liste des segments
         */
         
         create table #TMP_SEGT
         ( numero	smallint not null,
           num		smallint not null,
           autoroute	tinyint not null,
           coefd	int not null,
           coeff	int not null,
           sens		tinyint,
           nbvoies	tinyint,
           lBAU		tinyint,
           tunnel	bit,
           nom		char(25) )
           
           
         /*A
         ** Recherche de l'autoroute sur lequel se trouve la portion
         */
         
         select @vl_Autoroute = autoroute, @vl_PRdebut = PR_debut, @vl_PRfin = PR_fin from CFT..RES_POR
           where numero = @va_NumeroPortion_in
         
         /*A
         ** Declaration d'un curseur sur la table RES_SEG
         */
         
         select * into #TMP_SEGMT from CFT..RES_SEG 
           where portion = @va_NumeroPortion_in
           order by sens, PR_debut
         
         declare Segment cursor 
         for select portion, PR_debut, sens, nombre_de_voies, largeur_BAU, tunnel, nom_tunnel from #TMP_SEGMT
               where portion   = @va_NumeroPortion_in
	         
	 open Segment
	         
	 fetch Segment into @vl_Segment, @vl_PRd, @vl_Sens, @vl_NbVoies, @vl_BAU, @vl_Tunnel, @vl_NomTube
	 
	 /*A
	 ** Tant qur le curseur n'est pas vide
	 */
	 
	 while (@@sqlstatus = 0)
	 begin
	   
	    /*A
	    ** Recherche du PR de fin de segment :
	    ** PRfin(segment) = Prdebut(segment suivant)
	    */
	    
	    select @vl_PRf = min(PR_debut) from CFT..RES_SEG
	      where portion  = @va_NumeroPortion_in
	        and sens     = @vl_Sens
	        and PR_debut > @vl_PRd
	    
	   if @vl_PRf = null
	    
	       select @vl_PRf = PR_fin from CFT..RES_POR
	         where numero = @va_NumeroPortion_in
	         
	     /*A
	    ** Calcul des coefficients de translation d'un segment
	    */
	    
	    /*B
	    ** Le PRdebut de la portion est inferieur ou egal au PRdebut du segment
	    */
	    
	    if @vl_PRdebut <= @vl_PRd
	    begin
	       
	       select @vl_Coefd = (@vl_PRd - @vl_PRdebut) * 10000 / (@vl_PRfin - @vl_PRdebut )
	       
	       select @vl_Coeff = (@vl_PRf - @vl_PRdebut) * 10000 / (@vl_PRfin - @vl_PRdebut )
	       
	    end
	    
	    else
	    
	    /*B
	    ** Le PRdebut de la portion est superieur au PRdebut du segment
	    */
	    
	    begin
	       
	       select @vl_Coefd = (@vl_PRf - @vl_PRfin ) * 10000 / (@vl_PRdebut - @vl_PRfin )
	       
	       select @vl_Coeff = (@vl_PRd - @vl_PRfin ) * 10000 / (@vl_PRdebut - @vl_PRfin )
	    
	    end
	    
	    /*A
	    ** Insertion des infos pour la liste des segments dans la table temporaire TMP_SEG
	    */
	    
	    insert into #TMP_SEGT
	    values ( @vl_Num + 1, @vl_Segment, @vl_Autoroute, @vl_Coefd, @vl_Coeff, @vl_Sens, @vl_NbVoies, @vl_BAU, @vl_Tunnel, @vl_NomTube )
	    
	    fetch Segment into @vl_Segment, @vl_PRd, @vl_Sens, @vl_NbVoies, @vl_BAU, @vl_Tunnel, @vl_NomTube
	 
	 end
	 
	 /*A
	 ** Fermeture du curseur sur la table RES_SEG
	 */
	 
	 close Segment
	 
	 /*A
	 ** Renvoi la liste des segments
	 */
	 
	 select * from #TMP_SEGT
	    
      end
#endif      
      
   end
      
   return XDC_OK
         
go  
