# Repository Guidelines

## Project Structure & Module Organization
- `src/`: application code (modules, components, services). Example: `src/main.ts`, `src/components/CanvasPanel.tsx`.
- `tests/`: unit/integration tests mirroring `src/` paths. Example: `tests/components/CanvasPanel.test.ts`.
- `assets/`: static files (images, fonts, sample data).
- `scripts/`: local helpers (build, release, data prep).
- `config/` or `.*rc`: tool configs (lint, format, test). Adjust paths to match existing layout.

## Build, Test, and Development Commands
- `npm run dev` (or `pnpm/yarn dev`): start local dev with hot reload.
- `npm test` (or `pnpm/yarn test`): run unit tests once.
- `npm run test:watch`: re-run tests on change.
- `npm run build`: produce production build to `dist/`.
- `npm run lint` / `npm run format`: check/format code per repo config.
If the project uses Python, prefer `pytest`, `ruff`, and `black`: `pytest -q`, `ruff check .`, `black .`.

## Coding Style & Naming Conventions
- Indentation: 2 spaces for JS/TS, 4 for Python. No tabs.
- Naming: `camelCase` variables/functions, `PascalCase` classes/components, `SCREAMING_SNAKE_CASE` constants, `kebab-case` filenames except React components (`PascalCase.tsx`).
- Lint/format: ESLint + Prettier for JS/TS; Ruff + Black for Python. Run before commit.

## Testing Guidelines
- Framework: Jest/Vitest for JS/TS; Pytest for Python.
- Location: mirror `src/` under `tests/` or co-locate with `.test/.spec` files.
- Naming: `*.test.ts` / `*_test.py`. Include edge cases and error paths.
- Coverage: aim ≥ 80% lines/branches for changed code. Add fixtures under `tests/fixtures/`.

## Commit & Pull Request Guidelines
- Commits: follow Conventional Commits. Examples:
  - `feat(canvas): add brush size slider`
  - `fix(renderer): prevent NaN on empty spectrum`
  - `chore(ci): speed up cache restore`
- PRs: include purpose, linked issues (`Closes #123`), screenshots/CLI output, and test notes. Keep focused and small. Pass CI, lint, and tests.

## Security & Configuration Tips
- Never commit secrets. Use `.env` and `/.env.example`; add sensitive keys to `.gitignore`.
- Validate untrusted inputs; prefer centralized schema validation.
- Review third-party licenses before adding dependencies.

## Architecture Overview (Brief)
- Typical entry: `src/main.*` bootstraps app; feature modules live in `src/features/*`; shared utilities in `src/lib/*`. Prefer dependency-injected services and pure functions for testability.
