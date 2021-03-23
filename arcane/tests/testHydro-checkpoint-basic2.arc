<?xml version="1.0" ?>
<case codename="ArcaneTest" xml:lang="en" codeversion="1.0">
 <arcane>
  <title>Test les protections reprise en changeant le nombre de sous-domaines</title>
  <timeloop>ArcaneHydroLoop</timeloop>
 </arcane>

 <mesh>

  <file internal-partition="true">sod.vtk</file>

 <initialisation>
  <variable nom="Density" valeur="1." groupe="ZG" />
  <variable nom="Pressure" valeur="1." groupe="ZG" />
  <variable nom="AdiabaticCst" valeur="1.4" groupe="ZG" />
  <variable nom="Density" valeur="0.125" groupe="ZD" />
  <variable nom="Pressure" valeur="0.1" groupe="ZD" />
  <variable nom="AdiabaticCst" valeur="1.4" groupe="ZD" />
 </initialisation>
 </mesh>

 <arcane-post-processing>
   <output-period>5</output-period>
   <!-- <format name="EnsightHdfPostProcessor" /> -->
   <output>
    <variable>CellMass</variable>
    <variable>CellVolume</variable>
    <variable>Pressure</variable>
    <variable>Density</variable>
    <variable>Velocity</variable>
    <variable>NodeMass</variable>
    <variable>InternalEnergy</variable>
    <group>ZG</group>
    <group>ZD</group>
    <group>AllFaces</group>
    <group>XMIN</group>
    <group>XMAX</group>
    <group>YMIN</group>
    <group>YMAX</group>
    <group>ZMIN</group>
    <group>ZMAX</group>
   </output>
 </arcane-post-processing>
 <arcane-checkpoint>
  <checkpoint-service name="ArcaneBasic2CheckpointWriter" />
  <do-dump-at-end>true</do-dump-at-end>
 </arcane-checkpoint>

 <!-- Configuration du module hydrodynamique -->
 <simple-hydro>

   <deltat-init>0.001</deltat-init>
   <deltat-min>0.0001</deltat-min>
   <deltat-max>0.01</deltat-max>
   <final-time>0.2</final-time>

   <viscosity>cell</viscosity>
   <viscosity-linear-coef>0.5</viscosity-linear-coef>
   <viscosity-quadratic-coef>0.6</viscosity-quadratic-coef>

   <boundary-condition>
     <surface>XMIN</surface><type>Vx</type><value>0.</value>
   </boundary-condition>
   <boundary-condition>
     <surface>XMAX</surface><type>Vx</type><value>0.</value>
   </boundary-condition>
   <boundary-condition>
     <surface>YMIN</surface><type>Vy</type><value>0.</value>
   </boundary-condition>
   <boundary-condition>
     <surface>YMAX</surface><type>Vy</type><value>0.</value>
   </boundary-condition>
   <boundary-condition>
     <surface>ZMIN</surface><type>Vz</type><value>0.</value>
   </boundary-condition>
   <boundary-condition>
     <surface>ZMAX</surface><type>Vz</type><value>0.</value>
   </boundary-condition>
 </simple-hydro>
</case>
