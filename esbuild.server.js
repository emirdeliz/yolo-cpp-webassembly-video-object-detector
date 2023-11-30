const esbuild = require('esbuild');

esbuild.build({
  entryPoints: ['src/server/index.ts'],
  bundle: true,
  outfile: 'dist/server.js',
  platform: 'node',
});