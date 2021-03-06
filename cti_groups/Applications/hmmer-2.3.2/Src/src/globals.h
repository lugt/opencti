/************************************************************
 * HMMER - Biological sequence analysis with profile HMMs
 * Copyright (C) 1992-2003 Washington University School of Medicine
 * All Rights Reserved
 * 
 *     This source code is distributed under the terms of the
 *     GNU General Public License. See the files COPYING and LICENSE
 *     for details.
 ************************************************************/

/* globals.h
 * Mon Nov 18 13:05:03 1996
 * 
 * Global variable definitions. 
 * This file may only be included in a main() .c file.
 * 
 * Alphabet[] is usually treated as an array, but may be treated as
 * strings w/ strchr() calls to find index of a particular character,
 * so they must be null terminated.  Hence the +1.
 * [bug #h25. xref STL7 p121] 
 */

char  Alphabet[MAXCODE+1]; /* ACGT, for instance                    */ 
int   Alphabet_type;       /* hmmNUCLEIC or hmmAMINO                */
int   Alphabet_size;       /* uniq alphabet size: 4 or 20           */
int   Alphabet_iupac;      /* total size of alphabet + IUPAC degen. */
char  Degenerate[MAXCODE][MAXABET]; /* 1/0 arrays, for whether IUPAC code includes a residue */
int   DegenCount[MAXCODE];

