CREATE TABLE journal_entries (
    id TEXT PRIMARY KEY,
    tags TEXT,
    strategy TEXT,
    confidence INTEGER CHECK (confidence BETWEEN 1 AND 5),
    emotions TEXT,
    lessons TEXT,
    title TEXT,
    notes TEXT,
    screenshot_path TEXT
);

CREATE TABLE trades (
    id TEXT PRIMARY KEY,
    trade_type TEXT NOT NULL, -- 'futures', 'stock', 'option'
    symbol TEXT,
    account TEXT,
    quantity INTEGER,
    transaction_price REAL,
    closing_price REAL,
    realized_pnl REAL,
    mtm_pnl REAL,
    code TEXT,
    trade_direction TEXT,
    date TEXT,
    time TEXT
);