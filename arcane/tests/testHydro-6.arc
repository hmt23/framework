<?xml version="1.0"?>
<case codename="ArcaneTest" xml:lang="en" codeversion="1.0">
 <arcane>
  <title>Tube à choc de Sod avec table de marche</title>
  <timeloop>ArcaneHydroLoop</timeloop>
 </arcane>

 <mesh>

  <!-- <file internal-partition="true">sod.vtk</file> -->
  <meshgenerator><sod><x>50</x><y>5</y><z>5</z></sod></meshgenerator>

 <initialisation>
  <variable nom="Density" valeur="1." groupe="ZG" />
  <variable nom="Pressure" valeur="1." groupe="ZG" />
  <variable nom="AdiabaticCst" valeur="1.4" groupe="ZG" />
  <variable nom="Density" valeur="0.125" groupe="ZD" />
  <variable nom="Pressure" valeur="0.1" groupe="ZD" />
  <variable nom="AdiabaticCst" valeur="1.4" groupe="ZD" />
 </initialisation>
 </mesh>

 <functions>
  <table name="table-dt" parameter="time" value="real" interpolation="linear">
   <value><x>0.0</x><y>1.0e-3</y></value>
   <value><x>1.0e-2</x><y>1.0e-5</y></value>
  </table>
 </functions>

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
   <!-- <ensight7gold>
    <binary-file>true</binary-file>
   </ensight7gold>-->
 </arcane-post-processing>
 <arcane-checkpoint>
  <!--<period>1</period>  -->
  <!-- <checkpoint-service name="ArcaneHdf5MultiCheckpointWriter" /> -->
  <do-dump-at-end>true</do-dump-at-end>
 </arcane-checkpoint>

 <!-- Configuration du module hydrodynamique -->
 <simple-hydro>

   <deltat-init>   0.001   </deltat-init>
   <deltat-min>    0.000001   </deltat-min>
   <deltat-max function="table-dt">    0.01   </deltat-max>
   <final-time>     0.2    </final-time>

  <viscosity>cell</viscosity>
  <viscosity-linear-coef>    .5    </viscosity-linear-coef>
  <viscosity-quadratic-coef> .6    </viscosity-quadratic-coef>

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
