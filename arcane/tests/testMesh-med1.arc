<?xml version="1.0"?>
<cas codename="ArcaneTest" xml:lang="fr" codeversion="1.0">
 <arcane>
  <titre>Test Maillage MED1</titre>
  <description>Test Maillage MED1</description>
  <boucle-en-temps>UnitTest</boucle-en-temps>
 </arcane>

 <maillage>
  <fichier internal-partition="true">mesh1.med</fichier>
 </maillage>

 <module-test-unitaire>
   <test name="MeshUnitTest">
     <test-adjacence>0</test-adjacence>
   </test>
 </module-test-unitaire>

</cas>
