/*******************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: EvtGenBase
 *    File: $Id: EvtMacros.hh,v 1.1.1.2 2007/10/26 05:03:14 pingrg Exp $
 *  Author: Alexei Dvoretskii, dvoretsk@slac.stanford.edu, 2001-2002
 *
 * Copyright (C) 2002 Caltech
 *******************************************************************************/

// Useful macros.

// In copy constructors, deep copy pointers via CLONE
// and date members via COPY. 'other' is the name of the copied object 

#define COPY_PTR(X) X(other.X ? other.X->clone() : 0)
#define COPY_MEM(X) X(other.X)

