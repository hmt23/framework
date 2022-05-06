﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* main.cc                                                     (C) 2000-2022 */
/*                                                                           */
/* Main eos sample.                                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <arcane/Directory.h>
#include <arcane/launcher/ArcaneLauncher.h>

using namespace Arcane;

int
main(int argc,char* argv[])
{
  ArcaneLauncher::init(CommandLineArguments(&argc,&argv));
  auto& app_build_info = ArcaneLauncher::applicationBuildInfo();
  app_build_info.setCodeName("EOS");
  app_build_info.setCodeVersion(VersionInfo(1,0,0));
  // Positionne le chemin absolu de la DLL C# en considérant qu'elle
  // se trouve dans le répertoire de l'exécutable
  auto& dotnet_info = ArcaneLauncher::dotNetRuntimeInitialisationInfo();
  String exe_dir = ArcaneLauncher::getExeDirectory();
  dotnet_info.setMainAssemblyName(Directory(exe_dir).file("PerfectGas.dll"));
  return ArcaneLauncher::run();
}
