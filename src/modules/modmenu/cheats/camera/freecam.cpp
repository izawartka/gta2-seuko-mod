#include "freecam.h"
#include "../../../../events/game-tick.h"
#include "../../cheat-registry.h"

ModMenuModule::FreecamCheat* ModMenuModule::FreecamCheat::m_instance = nullptr;

ModMenuModule::FreecamCheat::FreecamCheat() : ModMenuModule::CheatBase("Cheat_Freecam_IsEnabled") {
	assert(m_instance == nullptr && "FreecamCheat instance already exists");
	m_instance = this;
}

ModMenuModule::FreecamCheat::~FreecamCheat()
{
	m_instance = nullptr;
}

ModMenuModule::FreecamCheat* ModMenuModule::FreecamCheat::GetInstance()
{
	assert(m_instance && "FreecamCheat not initialized!");
	return m_instance;
}

void ModMenuModule::FreecamCheat::ResetAndDisable()
{
	if (!IsEnabled()) {
		spdlog::error("FreecamCheat::ResetAndDisable: Cheat is not enabled, cannot reset and disable");
		return;
	}

	m_watchedControlState->RequestUpdate();
	m_isDisabling = true;
}

void ModMenuModule::FreecamCheat::OnFirstEnable()
{
	m_controlStateResolver = Core::MakeResolver(Game::Utils::GetPlayer, mem(&Game::Player::controlState));
}

void ModMenuModule::FreecamCheat::OnEnable()
{
	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();

	/// TODO: check GameTickEvent?
	m_watchedControlState = watchManager->Watch<GameTickEvent>(
		m_controlStateResolver,
		this,
		&ModMenuModule::FreecamCheat::OnControlStateChange
	);

	AddEventListener<GameEndEvent>(&ModMenuModule::FreecamCheat::OnGameEnd);
}

void ModMenuModule::FreecamCheat::OnDisable()
{
	RemoveEventListener<GameEndEvent>();

	Core::WatchManager* watchManager = Core::WatchManager::GetInstance();

	watchManager->Unwatch(m_watchedControlState);
	m_watchedControlState = nullptr;
	m_originalControlState = std::nullopt;
	m_resetAndDisableDone = false;

	m_isDisabling = false;
}

void ModMenuModule::FreecamCheat::OnControlStateChange(const std::optional<Game::PLAYER_CONTROL_STATE>& oldValue, const std::optional<Game::PLAYER_CONTROL_STATE>& newValue)
{
	if (m_isDisabling) {
		if (m_originalControlState.has_value()) {
			m_watchedControlState->SetValueNow(m_originalControlState.value(), false);
		}
		m_resetAndDisableDone = true;
		ResetAndDisableCheckAndProceed();
		return;
	}

	m_originalControlState = newValue;

	if (newValue.has_value()) {
		m_watchedControlState->SetValueNow(Game::PLAYER_CONTROL_STATE_FREECAM, false);
	}
}

void ModMenuModule::FreecamCheat::OnGameEnd(GameEndEvent& event)
{
	if (m_isDisabling) {
		SetEnabled(false);
	}
}

void ModMenuModule::FreecamCheat::ResetAndDisableCheckAndProceed()
{
	if (!m_isDisabling || !m_resetAndDisableDone) {
		return;
	}

	SetEnabled(false);
}

REGISTER_CHEAT(FreecamCheat)
