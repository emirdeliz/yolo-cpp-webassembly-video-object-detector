const esbuild = require('esbuild');

esbuild.build({
  entryPoints: ['src/client/index.ts'],
  bundle: true,
  watch: true,
  outfile: 'dist/client.js',
});