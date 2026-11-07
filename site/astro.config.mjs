// @ts-check
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';
import starlightSidebarTopics from 'starlight-sidebar-topics';
import starlightLinksValidator from 'starlight-links-validator';

// GitHub Pages (dépôt de projet) : https://<utilisateur>.github.io/<dépôt>/
// SITE_URL et SITE_BASE sont fournis par la CI ; valeurs locales par défaut.
const site = process.env.SITE_URL ?? 'http://localhost:4321';
const base = process.env.SITE_BASE ?? '/';

const group = (label, en, directory, extra = {}) => ({
  label, translations: { en }, items: [{ autogenerate: { directory, ...extra } }],
});
const sidebarFor = (v) => { const d = v.replace('.', '-'); return [
  group('Démarrer', 'Getting started', `${d}/getting-started`),
  group('Tutoriels', 'Tutorials', `${d}/tutorials`),
  group('Référence', 'Reference', `${d}/reference`, { collapsed: true }),
  ...(v === '3.0' ? [group('Migration 2.6 → 3.0', 'Migration 2.6 → 3.0', `${d}/migration`)] : []),
]; };

export default defineConfig({
  site,
  base,
  integrations: [
    starlight({
      title: 'CSFML — doc non officielle',
      description: 'Documentation communautaire de CSFML (SFML en C), versions 3.0 et 2.6, exemples testés.',
      defaultLocale: 'root',
      locales: {
        root: { label: 'Français', lang: 'fr' },
        en: { label: 'English', lang: 'en' },
      },
      social: [{ icon: 'github', label: 'GitHub', href: 'https://github.com/' }],
      customCss: ['./src/styles/custom.css'],
      plugins: [
        starlightLinksValidator(),
        starlightSidebarTopics([
          { label: 'CSFML 3.0', link: '/3-0/getting-started/first-window/', icon: 'rocket', items: sidebarFor('3.0') },
          { label: 'CSFML 2.6', link: '/2-6/getting-started/first-window/', icon: 'puzzle', items: sidebarFor('2.6') },
        ]),
      ],
    }),
  ],
});
