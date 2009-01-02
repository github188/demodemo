<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="classpath">
<!-- get dependencies -->
eclipse.lib.dependencies=\
<xsl:for-each select="classpathentry"><xsl:if test="@kind='lib'"><xsl:value-of select="@path"/>:</xsl:if></xsl:for-each>

<!-- get source locations -->
eclipse.project.src=\
<xsl:for-each select="classpathentry"><xsl:if test="@kind='src'"><xsl:if test="not(starts-with(@path, '/'))">
<xsl:value-of select="@path"/>:</xsl:if></xsl:if></xsl:for-each>

<!-- get eclipse dependent projects - assume workspace is in ../ -->
eclipse.project.dependencies=\
 <xsl:for-each select="classpathentry"><xsl:if test="@kind='src'"><xsl:if test="starts-with(@path, '/')">..<xsl:value-of
select="@path"/>,</xsl:if></xsl:if></xsl:for-each>
</xsl:template>
</xsl:stylesheet>


