// Chargement (avec cache) des fichiers d'API générés depuis les headers (api/csfml-<version>.json).
import fs from 'node:fs';
import path from 'node:path';
import tested from '../data/tested.json';

const cache = new Map();
export function loadApi(version) {
  if (!cache.has(version)) {
    const file = path.resolve(process.cwd(), '..', 'api', tested[version].api);
    cache.set(version, JSON.parse(fs.readFileSync(file, 'utf-8')));
  }
  return cache.get(version);
}
export const dirOf = (version) => version.replace('.', '-');
const kebab = (s) => s.replace(/([a-z0-9])([A-Z])/g, '$1-$2').toLowerCase();
// "CSFML/Graphics/RenderWindow.h" -> { module: 'graphics', slug: 'render-window' }
export function pageOf(header) {
  const [, mod, file] = header.split('/');
  return { module: (mod ?? '').toLowerCase(), slug: kebab((file ?? '').replace(/\.h$/, '')) };
}
export function functionPage(version, name, base = '/') {
  const fn = loadApi(version).functions.find((f) => f.name === name);
  if (!fn) return null;
  const { module, slug } = pageOf(fn.header);
  return `${base}${dirOf(version)}/reference/${module}/${slug}/#${name}`;
}
