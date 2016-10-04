#version 120

uniform sampler2DRect sourceTex; // passed in from openFrameworks
uniform sampler2DRect maskTex; // passed in from openFrameworks
void main() {
    
    vec4 coords = gl_TexCoord[0];
    
    vec4 srcFbo = texture2DRect(sourceTex, coords.xy);
    vec4 maskFbo = texture2DRect(maskTex, coords.xy);
    vec4 colFinal;
    // Output final pixel color
    //pass through yellow, red, and blue.
    colFinal = srcFbo;
    
//    if (maskFbo.r == 1){
//        colFinal.r = 1;
//        colFinal.g = 0;
//        colFinal.b = 0;
//        if (maskFbo.g == 1){
//            colFinal.g = 1;
//        }
//    }else if (maskFbo.b == 1){
//        colFinal.r = 0;
//        colFinal.g = 0;
//        colFinal.b = 1;
//    }else if (maskFbo.a == 1){
//        colFinal.r = 0;
//        colFinal.g = 0;
//        colFinal.b = 0;
//    }
   
   // gl_FragColor = colFinal;
    gl_FragColor = vec4(srcFbo.r , srcFbo.g * (1. - maskFbo.r), srcFbo.b * (1. - maskFbo.r), srcFbo.a);

}