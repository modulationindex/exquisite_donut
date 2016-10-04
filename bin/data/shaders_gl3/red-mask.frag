#version 150

uniform sampler2DRect sourceTex; // passed in from openFrameworks
uniform sampler2DRect maskTex; // passed in from openFrameworks


in vec2 texCoordVarying;

out vec4 fragColor;

void main(){
    vec4 coords = gl_TexCoord[0];

    vec4 srcFbo = texture2DRect(sourceTex, coords.xy);
    vec4 maskFbo = texture2DRect(maskTex, coords.xy);

    vec4 colFinal;
    colFinal = vec4(srcFbo.r,srcFbo.g,srcFbo.b,srcFbo.a * (1. - maskFbo.r));
    gl_FragColor = colFinal;

}
